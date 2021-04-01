//
// Created by quartzy on 3/29/21.
//

#include <iostream>
#include "PacketPlayerListItem.h"

#include "../../../MCCPP.h"

uint32_t network::PacketPlayerListItem::getPacketId() {
    return 0x38;
}

uint32_t network::PacketPlayerListItem::encode(uint8_t *out) {
    return 0;
}

void network::PacketPlayerListItem::decode(uint8_t *in, size_t size) {
    uint32_t i = 0;
    action = decode_unsigned_varint(in, i);
    uint32_t lenLen = 0;
    len = decode_unsigned_varint(in+i, lenLen);
    i+=lenLen;

    uuid = std::vector<UUID>{};
    uuid.reserve(len);
    switch (action) {
        case 0:
            addPlayer = std::vector<AddPlayer>{};
            addPlayer.reserve(len);
            break;
        case 1:
            updateGm = std::vector<uint8_t>{};
            updateGm.reserve(len);
            break;
        case 2:
            updatePing = std::vector<uint16_t>{};
            updatePing.reserve(len);
            break;
        case 3:
            updateDisplayName = std::vector<UpdateDisplayName>{};
            updateDisplayName.reserve(len);
            break;
    }
    for (int j = 0; j < len; ++j) {
        uint64_t mostSigBits = 0;
        uint64_t leastSigBits = 0;

        i+=decodeLong(in+i, mostSigBits);
        i+=decodeLong(in+i, leastSigBits);

        uuid.emplace_back(mostSigBits, leastSigBits);

        switch (action) {
            case 0:
            {
                AddPlayer addPlayerIns{};
                addPlayerIns.name = std::string{};
                i+=decodeString((const char*) in+i, addPlayerIns.name);
                uint32_t numPropertiesLen = 0;
                addPlayerIns.propertyNum = decode_unsigned_varint(in+i, numPropertiesLen);
                i+=numPropertiesLen;

                addPlayerIns.properties = std::vector<Property>{};
                addPlayerIns.properties.reserve(addPlayerIns.propertyNum);
                for (int k = 0; k < addPlayerIns.propertyNum; ++k) {
                    std::string propName{};
                    std::string propVal{};
                    bool isSigned;
                    std::string propSig{};

                    i+=decodeString((const char*) in+i, propName);
                    i+=decodeString((const char*) in+i, propVal);
                    isSigned = in[i++];
                    if (isSigned){
                        i+=decodeString((const char*) in+i, propSig);
                    }

                    addPlayerIns.properties.push_back({propName, propVal, isSigned, propSig});
                }

                uint32_t gmLen = 0;
                addPlayerIns.gamemode = decode_unsigned_varint(in+i, gmLen);
                i+=gmLen;

                addPlayerIns.hasDisplayName = in[i++];
                addPlayerIns.displayName = std::string{};
                if (addPlayerIns.hasDisplayName){
                    i+=decodeString((const char*) in+i, addPlayerIns.displayName);
                }
                addPlayer.push_back(addPlayerIns);
            }
            break;
            case 1:
            {
                uint32_t gmLen = 0;
                uint8_t gm = decode_unsigned_varint(in+i, gmLen);
                i+=gmLen;

                updateGm.push_back(gm);
            }
            break;
            case 2:
            {
                uint32_t pingLen = 0;
                uint16_t ping = decode_unsigned_varint(in+i, pingLen);
                i+=pingLen;

                updatePing.push_back(ping);
            }
            break;
            case 3:
            {
                UpdateDisplayName updateDisplayNameIns{};

                if ((updateDisplayNameIns.hasDisplayName = in[i++])==true){
                    i+=decodeString((const char*) in+i, updateDisplayNameIns.displayName);
                }

                updateDisplayName.push_back(updateDisplayNameIns);
            }
            break;
        }
    }
}

uint32_t network::PacketPlayerListItem::size() {
    return 0;
}

void network::PacketPlayerListItem::handlePacket() {
    switch (action) {
        case 0:
        {
            std::vector<AddPlayer> cpy = addPlayer;
            std::vector<UUID> cpyUUID = uuid;
            auto task = std::packaged_task<void()>{[=](){
                for (int i = 0;i<cpy.size();i++){
                    std::string name1 = cpy[i].name;
                    std::string displayName1 = cpy[i].displayName;
                    PlayerMP playerMp{name1, cpyUUID[i], cpy[i].gamemode, cpy[i].ping, cpy[i].hasDisplayName, displayName1};
                    MCCPP::players.push_back(playerMp);
                }
            }};
            MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
            std::cout << "Added players\n";
        }
        break;
        case 1:
        {
            std::vector<uint8_t> cpy = updateGm;
            std::vector<UUID> cpyUUID = uuid;
            auto task = std::packaged_task<void()>{[=](){
                for (int i = 0;i<MCCPP::players.size();i++){
                    for (int j = 0; j < cpyUUID.size(); ++j) {
                        if (cpyUUID[j]==MCCPP::players[i].getUuid()){
                            MCCPP::players[i].setGamemode(cpy[j]);
                            break;
                        }
                    }
                }
            }};
            MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
            std::cout << "Updated gamemodes\n";
        }
        break;
        case 2:
        {
            std::vector<uint16_t> cpy = updatePing;
            std::vector<UUID> cpyUUID = uuid;
            auto task = std::packaged_task<void()>{[=](){
                for (int i = 0;i<MCCPP::players.size();i++){
                    for (int j = 0; j < cpyUUID.size(); ++j) {
                        if (cpyUUID[j]==MCCPP::players[i].getUuid()){
                            MCCPP::players[i].setPing(cpy[j]);
                            break;
                        }
                    }
                }
            }};
            MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
            std::cout << "Updated ping\n";
        }
        break;
        case 3:
        {
            std::vector<UpdateDisplayName> cpy = updateDisplayName;
            std::vector<UUID> cpyUUID = uuid;
            auto task = std::packaged_task<void()>{[=](){
                for (int i = 0;i<MCCPP::players.size();i++){
                    for (int j = 0; j < cpyUUID.size(); ++j) {
                        if (cpyUUID[j]==MCCPP::players[i].getUuid()){
                            MCCPP::players[i].setHasDisplayName(cpy[j].hasDisplayName);
                            MCCPP::players[i].setDisplayName(cpy[j].displayName);
                            break;
                        }
                    }
                }
            }};
            MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
            std::cout << "Updated display name\n";
        }
        break;
        case 4:
        {
            std::vector<UUID> cpyUUID = uuid;
            auto task = std::packaged_task<void()>{[=](){
                for (int i = 0;i<MCCPP::players.size();i++){
                    for (int j = 0; j < cpyUUID.size(); ++j) {
                        if (cpyUUID[j]==MCCPP::players[i].getUuid()){
                            MCCPP::players.erase(MCCPP::players.begin() + i);
                            break;
                        }
                    }
                }
            }};
            MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
            std::cout << "Updated display name\n";
        }
        break;
    }
}
