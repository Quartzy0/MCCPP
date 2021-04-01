//
// Created by quartzy on 3/30/21.
//

#include "PacketPlayerPosAndLook.h"
#include "../../../MCCPP.h"
#include "../../../util/Log.h"
#include <cmath>

uint32_t network::PacketPlayerPosAndLook::getPacketId() {
    return 0x08;
}

uint32_t network::PacketPlayerPosAndLook::encode(uint8_t *out) {
    return 0;
}

void network::PacketPlayerPosAndLook::decode(uint8_t *in, size_t size) {
    uint32_t i = decodeDouble(in, x);
    i+=decodeDouble(in+i, y);
    i+=decodeDouble(in+i, z);
    i+=decodeFloat(in+i, yaw);
    i+=decodeFloat(in+i, pitch);
    flags = in[i++];
}

uint32_t network::PacketPlayerPosAndLook::size() {
    return 0;
}

void network::PacketPlayerPosAndLook::handlePacket() {
    auto task = std::packaged_task<void()>{[=](){
        glm::vec<3, double, glm::defaultp> newPos = glm::vec<3, double, glm::defaultp>{x,y,z};
        glm::vec<3, double, glm::defaultp> currentPos = MCCPP::playerController->getPos();

        newPos.x += currentPos.x * ((flags & 0x01)>0);
        newPos.y += currentPos.y * ((flags & 0x02)>0);
        newPos.z += currentPos.z * ((flags & 0x04)>0);

        float newHorizontal = (yaw*M_PI)/180.0f;
        float newVertical = (pitch*M_PI)/180.0f;

        newHorizontal += MCCPP::playerController->getCamera().getHorizontalAngle() * ((flags & 0x08)>0);
        newVertical += MCCPP::playerController->getCamera().getVerticalAngle() * ((flags & 0x10)>0);

        MCCPP::playerController->setPos(newPos);
        MCCPP::playerController->getCamera().setHorizontalAngle(newHorizontal);
        MCCPP::playerController->getCamera().setVerticalAngle(newVertical);

        CORE_INFO("New position is x: {}, y: {}, z: {} and rotation yaw: {}, pitch: {}", newPos.x, newPos.y, newPos.z, newHorizontal, newVertical);
    }};
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
