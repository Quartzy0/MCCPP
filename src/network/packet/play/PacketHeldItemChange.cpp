//
// Created by quartzy on 3/29/21.
//

#include <iostream>
#include "PacketHeldItemChange.h"
#include "../../../MCCPP.h"

uint32_t network::PacketHeldItemChange::getPacketId() {
    return 0x09;
}

uint32_t network::PacketHeldItemChange::encode(uint8_t *out) {
    return 0;
}

void network::PacketHeldItemChange::decode(uint8_t *in, size_t size) {
    slot = in[0];
}

uint32_t network::PacketHeldItemChange::size() {
    return 0;
}

void network::PacketHeldItemChange::handlePacket() {
    auto task = std::packaged_task<void()>([this](){
        MCCPP::playerController->setSelectedSlot(slot);
        std::cout << "Selected slot: " << std::to_string(slot) << "\n";
    });
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
