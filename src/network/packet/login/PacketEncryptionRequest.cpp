//
// Created by quartzy on 3/13/21.
//

#include <random>
#include <iostream>
#include "PacketEncryptionRequest.h"
#include "../../digest.h"
#include "PacketEncryptionResponse.h"
#include "../../HTTPRequest.hpp"
#include "../../NetworkManager.h"
#include "../../../MCCPP.h"

namespace network{

    uint32_t network::PacketEncryptionRequest::getPacketId() {
        return 1;
    }

    void network::PacketEncryptionRequest::decode(uint8_t *in, size_t size) {
        uint32_t i = decodeString((const char*) in, serverId);

        uint32_t publicKeyLenBytes = 0;
        publicKeyLen = decode_unsigned_varint(in+i, publicKeyLenBytes);
        i+=publicKeyLenBytes;
        publicKey = std::vector<uint8_t>{};
        publicKey.resize(publicKeyLen);
        std::memcpy(publicKey.data(), in+i, publicKeyLen);
        i+=publicKeyLen;

        uint32_t verifyTokenLenBytes = 0;
        verifyTokenLen = decode_unsigned_varint(in+i, verifyTokenLenBytes);
        i+=verifyTokenLenBytes;
        verifyToken = std::vector<uint8_t>{};
        verifyToken.resize(verifyTokenLen);
        std::memcpy(verifyToken.data(), in+i, verifyTokenLen);
        i+=verifyTokenLen;
    }

    const std::string &PacketEncryptionRequest::getServerId() const {
        return serverId;
    }

    uint32_t PacketEncryptionRequest::getPublicKeyLen() const {
        return publicKeyLen;
    }

    const std::vector<uint8_t> &PacketEncryptionRequest::getPublicKey() const {
        return publicKey;
    }

    uint32_t PacketEncryptionRequest::getVerifyTokenLen() const {
        return verifyTokenLen;
    }

    const std::vector<uint8_t> &PacketEncryptionRequest::getVerifyToken() const {
        return verifyToken;
    }

    uint32_t PacketEncryptionRequest::encode(uint8_t *out) {
        return 0;
    }

    uint32_t PacketEncryptionRequest::size() {
        return 0;
    }

    void PacketEncryptionRequest::handlePacket() {
        std::cout << "Beginning encryption\n";
        daft_hash_impl hasher;

        std::vector<unsigned char> sharedSecret(16);

        std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned char> engine;
        std::generate(begin(sharedSecret), end(sharedSecret), std::ref(engine));

        hasher.update(getServerId());
        hasher.update(sharedSecret.data(), sharedSecret.size());
        hasher.update(getPublicKey().data(), getPublicKeyLen());
        std::string result = hasher.finalise();

        std::string body = std::string{"{\"accessToken\":\""} + MCCPP::authToken + std::string{"\",\"selectedProfile\":\""} + MCCPP::uuid + std::string{"\",\"serverId\":\""} + result + std::string{"\"}"};
        std::string s = http::post("https://sessionserver.mojang.com/session/minecraft/join", body.c_str());

        PacketEncryptionResponse encryptionResponse{16, sharedSecret, getVerifyTokenLen(),
                                                    getVerifyToken(), getPublicKey()};

        NetworkManager::sendPacket(encryptionResponse);

        //Begin encryption
        NetworkManager::enableEncryption(sharedSecret);
    }
}