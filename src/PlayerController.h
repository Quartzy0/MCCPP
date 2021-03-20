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
public:
    PlayerController(Camera &camera, Superchunk &superchunk, Window &window);

    void tick(float deltaTime);

    glm::vec3 getLookingBlock(bool blockBefore);
};


#endif //MCCPP_PLAYERCONTROLLER_H
