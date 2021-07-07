//
// Created by quartzy on 3/31/21.
//

#include "PacketStatistics.h"
#include "../../../../MCCPP.h"
#include "../../../../util/Log.h"

uint32_t network::PacketStatistics::getPacketId() {
    return 0x37;
}

void network::PacketStatistics::encode(uint8_t *out) {

}

void network::PacketStatistics::decode(uint8_t *in, size_t size) {
    uint32_t i = 0;
    count = decode_unsigned_varint(in, i);

    for (uint32_t j = 0; j < count; ++j) {
        std::string name{};
        i+=decodeString((const char*) in+i, name);
        uint32_t valLen = 0;
        int32_t val = decode_signed_varint(in+i, valLen);
        i+=valLen;
        stats[name] = val;
    }
}

uint32_t network::PacketStatistics::size() {
    return 0;
}

void network::PacketStatistics::handlePacket() {
    std::unordered_map<std::string, int32_t> cpy = stats;
    auto task = std::packaged_task<void()>{[=](){
        MCCPP::statistics = cpy;
        CORE_INFO("Received {} statistics", cpy.size());
    }};
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
