//
// Created by quartzy on 3/30/21.
//

#include "PacketPlayerAbilities.h"
#include "../../../../MCCPP.h"
#include "../../../../util/Log.h"

uint32_t network::PacketPlayerAbilities::getPacketId() {
    return 0x39;
}

void network::PacketPlayerAbilities::encode(uint8_t *out) {

}

void network::PacketPlayerAbilities::decode(uint8_t *in, size_t size) {
    uint32_t i = 0;
    flags = in[i++];
    flySpeed = 0;
    walkSpeed = 0;
    i+=decodeFloat(in+i, flySpeed);
    i+=decodeFloat(in+i, walkSpeed);
}

uint32_t network::PacketPlayerAbilities::size() {
    return 0;
}

void network::PacketPlayerAbilities::handlePacket() {
    auto task = std::packaged_task<void()>{[this](){
//        MCCPP::playerController->setFlySpeed(flySpeed);
        MCCPP::playerController->setWalkSpeed(walkSpeed);

        MCCPP::playerController->setInvulnerable((flags & 0x01) > 0);
        MCCPP::playerController->setFlying((flags & 0x02) > 0);
        MCCPP::playerController->setAllowFlight((flags & 0x04) > 0);
        MCCPP::playerController->setCreative((flags & 0x08) > 0);

        CORE_INFO("Fly speed: {}, Walk speed: {}, Invulnerable: {}, Flying: {}, Allow flight: {}, Creative: {}", flySpeed, walkSpeed, MCCPP::playerController->isInvulnerable(), MCCPP::playerController->isFlying(), MCCPP::playerController->isAllowFlight(), MCCPP::playerController->isCreative());
    }};
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
