//
// Created by quartzy on 3/13/21.
//

#include <string>
#include <cstring>
#include <iostream>
#include "NetworkManager.h"
#include "Encryption.hpp"
#include "packet/login/PacketEncryptionRequest.h"
#include "packet/login/PacketSetCompression.h"
#include "packet/login/PacketLoginSuccess.h"
#include "packet/login/PacketDisconnect.h"
#include "packet/play/clientbound/PacketJoinGame.h"
#include "packet/play/clientbound/PacketKeepAlive.h"
#include "packet/play/clientbound/PacketPluginMessage.h"
#include "packet/play/clientbound/PacketServerDifficulty.h"
#include "packet/play/clientbound/PacketSpawnPosition.h"
#include "packet/play/clientbound/PacketHeldItemChange.h"
#include "packet/play/clientbound/PacketPlayerListItem.h"
#include "packet/play/clientbound/PacketPlayerAbilities.h"
#include "packet/play/clientbound/PacketPlayerPosAndLook.h"
#include "packet/play/clientbound/PacketStatistics.h"
#include "packet/play/clientbound/PacketTimeUpdate.h"
#include "packet/play/clientbound/PacketMapChunkBulk.h"

#include <zlib.h>
#include <iomanip>

namespace network{

    bool network::NetworkManager::encryptionEnabled;
    Socket network::NetworkManager::sock;
    std::thread network::NetworkManager::networkThread;
    State network::NetworkManager::state;
    bool network::NetworkManager::compressionEnabled;
    uint32_t network::NetworkManager::compressionThreshold;
    bool network::NetworkManager::stopped;
    bool network::NetworkManager::shouldStop;

    void NetworkManager::init(const char *serverIp, uint32_t port) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port); /*converts short to
                                       short with network byte order*/
        addr.sin_addr.s_addr = inet_addr(serverIp);
        if (sock==-1){
            std::cout << "error" << "\n";
        }
        if (connect(sock, (struct sockaddr*) &addr, sizeof(addr))==-1){
            std::cout << "error connecting" << "\n";
        }

        stopped = false;
        shouldStop = false;

        networkThread = std::thread{[&](){
            while (!NetworkManager::shouldStop){
                NetworkManager::handlePacket();
            }
            NetworkManager::stopped = true;
        }};
    }

    void NetworkManager::sendData(std::vector<uint8_t>& data, uint32_t packetId) {
        if (!compressionEnabled){
            uint32_t totalPacketSize = varIntLen(packetId) + data.size();

            uint8_t sendBuf[totalPacketSize+varIntLen(totalPacketSize)];
            int i = encodeVarInt(totalPacketSize, sendBuf);
            i+=encodeVarInt(packetId, sendBuf+i);
            std::memcpy(sendBuf+i, data.data(), data.size());

            ssize_t bytesSend = send(sock, sendBuf, totalPacketSize+varIntLen(totalPacketSize), 0);
            if (bytesSend == -1) {
                perror("Send error");
                close(sock);
            }
        }else if(data.size()<compressionThreshold){
            uint32_t totalPacketSize = varIntLen(packetId) + data.size() + varIntLen(0);

            uint8_t sendBuf[totalPacketSize+varIntLen(totalPacketSize)];
            int i = encodeVarInt(totalPacketSize, sendBuf);
            i+=encodeVarInt(0, sendBuf+i);
            i+=encodeVarInt(packetId, sendBuf+i);
            std::memcpy(sendBuf+i, data.data(), data.size());

            ssize_t bytesSend = send(sock, sendBuf, totalPacketSize+varIntLen(totalPacketSize), 0);
            if (bytesSend == -1) {
                perror("Send error");
                close(sock);
            }
        }else{
            //Make uncompressed data
            uint32_t uncompressedSize = varIntLen(packetId) + data.size();

            uint8_t uncompressedData[uncompressedSize];
            int i1 = encodeVarInt(packetId, uncompressedData);
            std::memcpy(uncompressedData + i1, data.data(), data.size());

            //Compress the packet
            std::vector<uint8_t> compressedData{};
            uint64_t compressedSize = uncompressedSize;
            compressedData.resize(compressedSize);

            compress((Bytef*)compressedData.data(), &compressedSize, (const Bytef*)uncompressedData, uncompressedSize);
            compressedData.resize(compressedSize);

            uint8_t sendBuf[varIntLen(varIntLen(uncompressedSize) + compressedSize) + varIntLen(uncompressedSize) + compressedSize];
            int i = encodeVarInt(varIntLen(varIntLen(uncompressedSize) + compressedSize), sendBuf);
            i+=encodeVarInt(uncompressedSize, sendBuf+i);
            std::memcpy(sendBuf + i, compressedData.data(), compressedSize);
            i+=compressedSize;

            ssize_t bytesSend = send(sock, sendBuf, i, 0);
            if (bytesSend == -1) {
                perror("Send error");
                close(sock);
            }
        }
    }

    void NetworkManager::receiveData(std::vector<uint8_t> &data, uint32_t &packetId) {
        uint8_t buf[BUFSIZE];
        ssize_t readden = recv(sock, buf, BUFSIZE, 0); /*read may be also used*/
        if (readden < 0) {
            perror("Receive error");
            close(sock);
            exit(1);
        }
        else if (readden == 0)
        {
            fprintf(stderr, "Client orderly shut down the connection.\n");
        }
        else {
            std::vector<uint8_t> in;
            if (encryptionEnabled){
                in = encryption::decrypt(buf, readden);
            }else{
                in = std::vector<uint8_t>{};
                in.resize(readden);
                std::memcpy(in.data(), &buf, readden);
            }

            if (!compressionEnabled){
                uint32_t packetLenBytes = -1;
                uint32_t packetLen = decode_unsigned_varint(in.data(), packetLenBytes);

                uint32_t packetIdBytes = -1;
                packetId = decode_unsigned_varint(in.data() + packetLenBytes, packetIdBytes);

                data.resize(packetLen-packetIdBytes);
                std::memcpy(data.data(), in.data() + packetLenBytes + packetIdBytes, packetLen-packetIdBytes);
            }else{
                uint32_t packetLenBytes = -1;
                uint32_t packetLen = decode_unsigned_varint(in.data(), packetLenBytes);

                uint32_t dataLenBytes = -1;
                uint32_t dataLen = decode_unsigned_varint(in.data() + packetLenBytes, dataLenBytes);

                if (dataLen==0){
                    uint32_t packetIdBytes = -1;
                    packetId = decode_unsigned_varint(in.data() + dataLenBytes + packetLenBytes, packetIdBytes);

                    if (packetLen==0 || packetLen>(BUFSIZE-packetLenBytes)){
                        packetId = -1;
                        std::cerr << "Received packet of inappropriate length! " << packetLen << " 0x" << std::setfill('0') << std::setw(2) << std::hex << packetId << "\n";
                        return;
                    }

                    data.resize(packetLen-dataLenBytes-packetIdBytes);
                    std::memcpy(data.data(), in.data() + packetLenBytes + dataLenBytes + packetIdBytes, packetLen-dataLenBytes-packetIdBytes);
                }else{
                    std::vector<uint8_t> uncompressedData{};
                    uncompressedData.resize(dataLen);
                    uLongf size = dataLen;
                    uncompress((Bytef*)uncompressedData.data(), &size, (const Bytef*)in.data() + packetLenBytes + dataLenBytes, dataLen);

                    uint32_t packetIdBytes = -1;
                    packetId = decode_unsigned_varint(uncompressedData.data(), packetIdBytes);

                    data.resize(dataLen-packetIdBytes);
                    std::memcpy(data.data(), uncompressedData.data() + packetIdBytes, dataLen-packetIdBytes);
                }
            }
        }
    }

    void NetworkManager::enableEncryption(std::vector<uint8_t> &ss) {
        uint8_t key[16];
        std::memcpy(key, ss.data(), 16);
        uint8_t iv[16];
        std::memcpy(iv, ss.data(), 16);

        encryption::init(key, iv);

        NetworkManager::encryptionEnabled = true;
    }

    void NetworkManager::sendPacket(Packet &packet) {
        std::vector<uint8_t> dataToSend{};
        dataToSend.resize(packet.size());
        packet.encode(dataToSend.data());

        NetworkManager::sendData(dataToSend, packet.getPacketId());
    }

    void NetworkManager::handlePacket() {
        std::vector<uint8_t> dataIn{};
        uint32_t packetId = -1;
        receiveData(dataIn, packetId);
        if (packetId==-1){
            return;
        }

        switch (state) {
            case LOGIN:
                switch (packetId) {
                    case 0:
                        {
                            std::shared_ptr<PacketDisconnect> packet = std::make_shared<PacketDisconnect>();
                            packet->decode(dataIn.data(), dataIn.size());
                            packet->handlePacket();
                        }
                        break;
                    case 1:
                        {
                            std::shared_ptr<PacketEncryptionRequest> packet = std::make_shared<PacketEncryptionRequest>();
                            packet->decode(dataIn.data(), dataIn.size());
                            packet->handlePacket();
                        }
                        break;
                    case 2:
                        {
                            std::shared_ptr<PacketLoginSuccess> packet = std::make_shared<PacketLoginSuccess>();
                            packet->decode(dataIn.data(), dataIn.size());
                            packet->handlePacket();
                        }
                        break;
                    case 3:
                        {
                            std::shared_ptr<PacketSetCompression> packet = std::make_shared<PacketSetCompression>();
                            packet->decode(dataIn.data(), dataIn.size());
                            packet->handlePacket();
                        }
                        break;
                    default:
                        std::cerr << "Unknown packet 0x" << std::setfill('0') << std::setw(2) << std::hex << packetId << "\n";
                        break;
                }
                break;
            case PLAY:
                switch (packetId) {
                    case 0:
                    {
                        std::shared_ptr<PacketKeepAlive> packet = std::make_shared<PacketKeepAlive>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 1:
                    {
                        std::shared_ptr<PacketJoinGame> packet = std::make_shared<PacketJoinGame>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x3F:
                    {
                        std::shared_ptr<PacketPluginMessage> packet = std::make_shared<PacketPluginMessage>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x41:
                    {
                        std::shared_ptr<PacketServerDifficulty> packet = std::make_shared<PacketServerDifficulty>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x05:
                    {
                        std::shared_ptr<PacketSpawnPosition> packet = std::make_shared<PacketSpawnPosition>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x09:
                    {
                        std::shared_ptr<PacketHeldItemChange> packet = std::make_shared<PacketHeldItemChange>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x38:
                    {
                        std::shared_ptr<PacketPlayerListItem> packet = std::make_shared<PacketPlayerListItem>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x39:
                    {
                        std::shared_ptr<PacketPlayerAbilities> packet = std::make_shared<PacketPlayerAbilities>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x08:
                    {
                        std::shared_ptr<PacketPlayerPosAndLook> packet = std::make_shared<PacketPlayerPosAndLook>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x37:
                    {
                        std::shared_ptr<PacketStatistics> packet = std::make_shared<PacketStatistics>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x03:
                    {
                        std::shared_ptr<PacketTimeUpdate> packet = std::make_shared<PacketTimeUpdate>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    case 0x26:
                    {
                        std::shared_ptr<PacketMapChunkBulk> packet = std::make_shared<PacketMapChunkBulk>();
                        packet->decode(dataIn.data(), dataIn.size());
                        packet->handlePacket();
                    }
                    break;
                    default:
//                        std::cerr << "Unknown packet 0x" << std::setfill('0') << std::setw(2) << std::hex << packetId << "\n";
                        break;
                }
                break;
            default:
                std::cerr << "Unknown packet 0x" << std::setfill('0') << std::setw(2) << std::hex << packetId << "\n";
                break;
        }
    }

    State NetworkManager::getState() {
        return state;
    }

    void NetworkManager::setState(State state) {
        NetworkManager::state = state;
    }

    void NetworkManager::enableCompression(uint32_t threshold) {
        compressionEnabled = true;
        compressionThreshold = threshold;
    }

    void NetworkManager::cleanup() {
        NetworkManager::shouldStop = true;
        networkThread.join();
        close(sock);
    }

}