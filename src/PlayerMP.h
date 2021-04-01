//
// Created by quartzy on 3/29/21.
//

#ifndef MCCPP_PLAYERMP_H
#define MCCPP_PLAYERMP_H

#include <string>
#include "util/UUID.h"

struct Property{
    std::string name;
    std::string val;
    bool isSigned;
    std::string sig;
};

class PlayerMP {
private:
    std::string username;
    UUID uuid;
    uint8_t gamemode;
    uint16_t ping;
    bool hasDisplayName;
    std::string displayName;

    size_t propertyCount;
    Property* properties;
public:
    PlayerMP(std::string &username, UUID uuid, uint8_t gamemode, uint16_t ping, bool hasDisplayName,
             std::string &displayName);

    const std::string &getUsername() const;

    const UUID &getUuid() const;

    uint8_t getGamemode() const;

    uint16_t getPing() const;

    bool isHasDisplayName() const;

    const std::string &getDisplayName() const;

    void setGamemode(uint8_t gamemode);

    void setPing(uint16_t ping);

    void setHasDisplayName(bool hasDisplayName);

    void setDisplayName(const std::string &displayName);
};


#endif //MCCPP_PLAYERMP_H
