//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKET_H
#define TESTING1_PACKET_H

#include <cstring>
#include <string>

namespace network {
    uint32_t decode_unsigned_varint( uint8_t* data, uint32_t &decoded_bytes );

    int32_t decode_signed_varint( uint8_t* data, uint32_t &decoded_bytes );

    int32_t encodeVarInt(int32_t val, uint8_t* buf);

    int32_t varIntLen(int32_t val);

    uint32_t encodeString(const char* val, uint8_t* buf);

    uint32_t strLen(const char* val);

    int32_t encodeShort(uint16_t val, uint8_t* buf);

    uint32_t decodeString(const char* buf, std::string& string);

    uint32_t decodeInt(uint8_t *buf, int32_t &val);

    uint32_t decodeLong(uint8_t* buf, uint64_t& val);

    uint32_t decodeBlockPos(uint8_t* buf, int32_t& x, int32_t& y, int32_t& z);

    uint32_t decodeFloat(uint8_t* buf, float& val);

    uint32_t decodeDouble(uint8_t* buf, double& val);

    uint32_t decodeLong(uint8_t* buf, int64_t& val);

    uint32_t decodeInt(uint8_t* buf, uint32_t& val);

    uint32_t decodeInt(uint8_t* buf, int32_t& val);

    uint32_t decodeShort(uint8_t* buf, uint16_t& val);

    uint32_t decodeShort(uint8_t* buf, int16_t& val);

    class Packet {
    public:

        virtual uint32_t getPacketId() = 0;

        virtual void encode(uint8_t *out) = 0;

        virtual void decode(uint8_t *in, size_t size) = 0;

        //Size if packet was encoded
        virtual uint32_t size() = 0;

        virtual void handlePacket() = 0;
    };
}

#endif //TESTING1_PACKET_H
