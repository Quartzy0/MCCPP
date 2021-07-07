//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKETENCRYPTIONRESPONSE_H
#define TESTING1_PACKETENCRYPTIONRESPONSE_H

#include <cstdint>
#include <vector>
#include "../Packet.h"

namespace network{

    class PacketEncryptionResponse : public Packet{
    private:
        uint32_t ssLen;
        std::vector<uint8_t> ss;
        uint32_t verifyTokenLen;
        std::vector<uint8_t> verifyToken;

        std::vector<uint8_t> publicKey;
    public:
        PacketEncryptionResponse(uint32_t ssLen, const std::vector<uint8_t> &ss, uint32_t verifyTokenLen,
                                 const std::vector<uint8_t> &verifyToken, const std::vector<uint8_t> &publicKey);

        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void handlePacket() override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

    };

}

#endif //TESTING1_PACKETENCRYPTIONRESPONSE_H
