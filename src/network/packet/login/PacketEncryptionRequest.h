//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKETENCRYPTIONREQUEST_H
#define TESTING1_PACKETENCRYPTIONREQUEST_H

#include <string>
#include <vector>
#include "../Packet.h"

namespace network{

    class PacketEncryptionRequest : public Packet{
    private:
        std::string serverId;
        uint32_t publicKeyLen;
        std::vector<uint8_t> publicKey;
        uint32_t verifyTokenLen;
        std::vector<uint8_t> verifyToken;
    public:
        PacketEncryptionRequest() = default;

        uint32_t getPacketId() override;

        void decode(uint8_t *in, size_t size) override;

        const std::string &getServerId() const;

        uint32_t getPublicKeyLen() const;

        const std::vector<uint8_t> &getPublicKey() const;

        void encode(uint8_t *out) override;

        uint32_t size() override;

        uint32_t getVerifyTokenLen() const;

        void handlePacket() override;

        const std::vector<uint8_t> &getVerifyToken() const;

    };

}
#endif //TESTING1_PACKETENCRYPTIONREQUEST_H
