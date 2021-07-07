//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKETHANDSHAKE_H
#define TESTING1_PACKETHANDSHAKE_H


#include <stdint-gcc.h>
#include <string>
#include "../Packet.h"

namespace network{

    class PacketHandshake : public Packet {
    private:
        uint32_t protocolVersion = 47;
        std::string serverAddress{};
        uint16_t port = 25565;
        //1 status, 2 login
        uint32_t nextState = 1;
    public:
        PacketHandshake(uint32_t protocolVersion, std::string& serverAddress, uint16_t port, uint32_t nextState);
        PacketHandshake() = default;

        void handlePacket() override;

        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;
    };

}


#endif //TESTING1_PACKETHANDSHAKE_H
