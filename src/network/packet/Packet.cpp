//
// Created by quartzy on 3/13/21.
//
#include <cmath>
#include <iostream>
#include <bitset>
#include "Packet.h"

namespace network{
    uint32_t decode_unsigned_varint( uint8_t* data, uint32_t &decoded_bytes )
    {
        uint32_t i = 0;
        uint64_t decoded_value = 0;
        int shift_amount = 0;

        do
        {
            decoded_value |= (uint64_t)(data[i] & 0x7F) << shift_amount;
            shift_amount += 7;
        } while ( (data[i++] & 0x80) != 0 );

        decoded_bytes = i;
        return decoded_value;
    }

    int32_t decode_signed_varint( uint8_t* data, uint32_t &decoded_bytes )
    {
        uint64_t unsigned_value = decode_unsigned_varint(data, decoded_bytes);
        return (int64_t)( unsigned_value & 1 ? ~(unsigned_value >> 1)
                                             :  (unsigned_value >> 1) );
    }

    int32_t encodeVarInt(int32_t val, uint8_t* buf){
        uint64_t uval = val;

        int encoded = 0;

        do {
            uint8_t nextByte = uval & 0x7F;
            uval >>= 7;
            if (uval)
                nextByte |= 0x80;
            buf[encoded++] = nextByte;
        } while (uval);

        return encoded;
    }

    int32_t varIntLen(int32_t val){
        uint64_t uval = val;

        int encoded = 0;

        do {
            uint8_t nextByte = uval & 0x7F;
            uval >>= 7;
            if (uval)
                nextByte |= 0x80;
            encoded++;
        } while (uval);

        return encoded;
    }

    uint32_t encodeString(const char* val, uint8_t* buf){
        std::string s = std::string{val};
        int lengthLen = encodeVarInt(s.length(), buf);
        std::memcpy(buf + lengthLen, val, s.length());
        return s.length() + varIntLen(s.length());
    }

    uint32_t strLen(const char* val){
        std::string s = std::string{val};
        return s.length() + varIntLen(s.length());
    }

    int32_t encodeShort(uint16_t val, uint8_t* buf){
        buf[0] = (val & 0x00FF);
        buf[1] = (val & 0xFF00) >> 8;
        return 2;
    }

    uint32_t decodeString(const char* buf, std::string& string){
        uint32_t decodedBytes = -1;
        int32_t strLen = decode_unsigned_varint((uint8_t*) buf, decodedBytes);
        string.assign(buf + decodedBytes, strLen);
        return decodedBytes + strLen;
    }

    uint32_t decodeLong(uint8_t* buf, uint64_t& val){
        std::memcpy(&val, buf, sizeof(uint64_t));
        return sizeof(uint64_t);
    }

    uint32_t decodeLong(uint8_t* buf, int64_t& val){
        std::memcpy(&val, buf, sizeof(int64_t));
        return sizeof(int64_t);
    }

    uint32_t decodeBlockPos(uint8_t* buf, int32_t& x, int32_t& y, int32_t& z){
        uint64_t val = 0;
        val = ((uint64_t) buf[7]) | ((uint64_t) buf[6]) << 8 | ((uint64_t) buf[5]) << 16 | ((uint64_t) buf[4]) << 24 | ((uint64_t) buf[3]) << 32 |
                ((uint64_t) buf[2]) << 40 | ((uint64_t) buf[1]) << 48 | ((uint64_t) buf[0]) << 56;

        x = val >> 38;
        y = (val >> 26) & 0xFFF;
        z = val << 38 >> 38;

        if (x >= (std::pow(2, 25))){
            x -= (int32_t) (std::pow(2, 26));
        }
        if (y >= (std::pow(2, 11))){
            y -= (int32_t) (std::pow(2, 12));
        }
        if (z >= (std::pow(2, 25))){
            z -= (int32_t) (std::pow(2, 26));
        }

        return sizeof(uint64_t);
    }

    uint32_t decodeFloat(uint8_t* buf, float& val){
        uint8_t flipped[sizeof(float)];
        for (int i = 0; i < sizeof(float); ++i) {
            flipped[i] = buf[sizeof(float)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(float));
        return sizeof(float);
    }

    uint32_t decodeDouble(uint8_t* buf, double& val){
        uint8_t flipped[sizeof(double)];
        for (int i = 0; i < sizeof(double); ++i) {
            flipped[i] = buf[sizeof(double)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(double));
        return sizeof(double);
    }

    uint32_t decodeInt(uint8_t* buf, uint32_t& val){
        uint8_t flipped[sizeof(uint32_t)];
        for (int i = 0; i < sizeof(uint32_t); ++i) {
            flipped[i] = buf[sizeof(uint32_t)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(uint32_t));
        return sizeof(uint32_t);
    }

    uint32_t decodeInt(uint8_t* buf, int32_t& val){
        uint8_t flipped[sizeof(int32_t)];
        for (int i = 0; i < sizeof(int32_t); ++i) {
            flipped[i] = buf[sizeof(int32_t)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(int32_t));
        return sizeof(int32_t);
    }

    uint32_t decodeShort(uint8_t* buf, uint16_t& val){
        uint8_t flipped[sizeof(uint16_t)];
        for (int i = 0; i < sizeof(uint16_t); ++i) {
            flipped[i] = buf[sizeof(uint16_t)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(uint16_t));
        return sizeof(uint16_t);
    }

    uint32_t decodeShort(uint8_t* buf, int16_t& val){
        uint8_t flipped[sizeof(int16_t)];
        for (int i = 0; i < sizeof(int16_t); ++i) {
            flipped[i] = buf[sizeof(int16_t)-(i+1)];
        }

        std::memcpy(&val, flipped, sizeof(int16_t));
        return sizeof(int16_t);
    }
}