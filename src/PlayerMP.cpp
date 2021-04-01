//
// Created by quartzy on 3/29/21.
//

#include "PlayerMP.h"

const std::string &PlayerMP::getUsername() const {
    return username;
}

const UUID &PlayerMP::getUuid() const {
    return uuid;
}

uint8_t PlayerMP::getGamemode() const {
    return gamemode;
}

uint16_t PlayerMP::getPing() const {
    return ping;
}

bool PlayerMP::isHasDisplayName() const {
    return hasDisplayName;
}

const std::string &PlayerMP::getDisplayName() const {
    return displayName;
}

void PlayerMP::setGamemode(uint8_t gamemode) {
    PlayerMP::gamemode = gamemode;
}

void PlayerMP::setPing(uint16_t ping) {
    PlayerMP::ping = ping;
}

void PlayerMP::setHasDisplayName(bool hasDisplayName) {
    PlayerMP::hasDisplayName = hasDisplayName;
}

void PlayerMP::setDisplayName(const std::string &displayName) {
    PlayerMP::displayName = displayName;
}

PlayerMP::PlayerMP(std::string &username, UUID uuid, uint8_t gamemode, uint16_t ping,
                   bool hasDisplayName, std::string &displayName) : username(std::move(username)), uuid(uuid),
                                                                          gamemode(gamemode), ping(ping),
                                                                          hasDisplayName(hasDisplayName),
                                                                          displayName(std::move(displayName)) {}
