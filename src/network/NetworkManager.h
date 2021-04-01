//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_NETWORKMANAGER_H
#define TESTING1_NETWORKMANAGER_H


#include <cstdint>
#include <vector>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501  /* Windows XP. */
    #endif
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    typedef unsigned int Socket;

#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int Socket;

#endif
#define BUFSIZE (4096*8)
#define PLAYER_UUID "3a407694cc2041c8a5c2cea0865ef885"
#define PLAYER_NAME "Quartzy0"
#define AUTH_TOKEN "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJlNWFlYzhlNTFjNzc0MjYzOGFhZDY5OTc5MGIzNmQzYSIsInlnZ3QiOiIwYmFlZGViYzFkNWU0ZGYxODVjMDE0MWRiYmQ5YTY4YSIsImlzcyI6IllnZ2RyYXNpbC1BdXRoIiwiZXhwIjoxNjE3MTAwOTYxLCJpYXQiOjE2MTY5MjgxNjF9.n9QV-XnxrjHtfFK99pVrm9uTLrGuCrncnzEBabFNBAo"

#include "packet/Packet.h"

#include <thread>

namespace network{

    enum State : uint32_t{
        HANDSHAKE = 0,
        STATUS = 1,
        LOGIN = 2,
        PLAY = 3
    };

    enum Side : uint32_t{
        SERVERBOUND = 0,
        CLIENTBOUND = 1
    };

    class NetworkManager {
    private:
        static bool shouldStop;
        static bool stopped;

        static bool encryptionEnabled;
        static uint32_t compressionThreshold;
        static bool compressionEnabled;

        static State state;

        static std::thread networkThread;

        static void handlePacket();

        static void receiveData(std::vector<uint8_t>& data, uint32_t& packetId);
    public:
        static Socket sock;

        static void init(const char* serverIp, uint32_t port);

        static void enableEncryption(std::vector<uint8_t>& ss);

        static void enableCompression(uint32_t threshold);

        static void sendPacket(Packet& packet);

        static void sendData(std::vector<uint8_t>& data, uint32_t packetId);

        static void cleanup();

        static State getState();

        static void setState(State state);
    };

}


#endif //TESTING1_NETWORKMANAGER_H
