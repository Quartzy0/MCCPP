//
// Created by quartzy on 3/14/21.
//

#ifndef TESTING1_PACKETLOGINSUCCESS_H
#define TESTING1_PACKETLOGINSUCCESS_H

#include "../Packet.h"
namespace network{

    class PacketLoginSuccess : public Packet{
    private:
        std::string username;
        std::string uuid;
    public:
        PacketLoginSuccess() = default;

        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //TESTING1_PACKETLOGINSUCCESS_H
