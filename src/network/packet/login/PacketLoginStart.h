//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKETLOGINSTART_H
#define TESTING1_PACKETLOGINSTART_H

#include <string>
#include "../Packet.h"
namespace network{
    class PacketLoginStart : public Packet{
    private:
        std::string username;
    public:
        PacketLoginStart(std::string& username);

        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };
}

#endif //TESTING1_PACKETLOGINSTART_H
