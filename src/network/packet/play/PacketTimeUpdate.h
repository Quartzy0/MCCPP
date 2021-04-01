//
// Created by quartzy on 3/31/21.
//

#ifndef MCCPP_PACKETTIMEUPDATE_H
#define MCCPP_PACKETTIMEUPDATE_H

#include "../Packet.h"

namespace network{

    class PacketTimeUpdate : public Packet{
    private:
        int64_t worldTime, timeOfDay;
    public:
        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETTIMEUPDATE_H
