//
// Created by quartzy on 3/29/21.
//

#ifndef MCCPP_PACKETPLAYERLISTITEM_H
#define MCCPP_PACKETPLAYERLISTITEM_H

#include <vector>
#include "../../Packet.h"
#include "../../../../PlayerMP.h"
#include "../../../../util/UUID.h"

namespace network{

    class PacketPlayerListItem : public Packet{
    private:

        struct AddPlayer{
            std::string name;
            size_t propertyNum;
            std::vector<Property> properties;
            uint8_t gamemode;
            uint16_t ping;
            bool hasDisplayName;
            std::string displayName;
        };

        struct UpdateDisplayName{
            bool hasDisplayName;
            std::string displayName;
        };

        uint8_t action;
        size_t len;

        std::vector<UUID> uuid;

        std::vector<AddPlayer> addPlayer;
        std::vector<uint8_t> updateGm;
        std::vector<uint16_t> updatePing;
        std::vector<UpdateDisplayName> updateDisplayName;
    public:

        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETPLAYERLISTITEM_H
