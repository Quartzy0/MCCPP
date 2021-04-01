//
// Created by quartzy on 2/20/21.
//

#ifndef MCCPP_PLAYERCONTROLLER_H
#define MCCPP_PLAYERCONTROLLER_H


#include "blocks/Superchunk.h"
#include "graphics/Camera.h"
#include "graphics/Window.h"

#define PLAYER_REACH 4

class PlayerController {
private:
    Camera& camera;
    Superchunk& superchunk;
    Window& window;

    glm::vec<3, double, glm::defaultp> pos;

    float mouseSpeed = 0.2f;
    float flySpeed = 10;
    float walkSpeed = 10;

    bool invulnerable, flying, allowFlight, creative;

    uint8_t selectedSlot;
public:
    PlayerController(Camera &camera, Superchunk &superchunk, Window &window);

    void tick(float deltaTime);

    glm::vec3 getLookingBlock(bool blockBefore);

    int32_t getX() const;

    void setX(int32_t x);

    int32_t getY() const;

    void setY(int32_t y);

    int32_t getZ() const;

    void setZ(int32_t z);

    const glm::vec<3, double, glm::defaultp> &getPos() const;

    uint8_t getSelectedSlot() const;

    void setSelectedSlot(uint8_t selectedSlot);

    float getMouseSpeed() const;

    void setMouseSpeed(float mouseSpeed);

    float getFlySpeed() const;

    void setFlySpeed(float flySpeed);

    float getWalkSpeed() const;

    Camera &getCamera() const;

    void setWalkSpeed(float walkSpeed);

    void setPos(const glm::vec<3, double, glm::defaultp> &pos);

    bool isInvulnerable() const;

    void setInvulnerable(bool invulnerable);

    bool isFlying() const;

    void setFlying(bool flying);

    bool isAllowFlight() const;

    void setAllowFlight(bool allowFlight);

    bool isCreative() const;

    void setCreative(bool creative);
};


#endif //MCCPP_PLAYERCONTROLLER_H
