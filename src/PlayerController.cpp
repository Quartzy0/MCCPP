//
// Created by quartzy on 2/20/21.
//

#include "PlayerController.h"
#include "util/Log.h"
#include "util/InputManager.h"

#include "glm/gtx/intersect.hpp"

float dti(float val) {
    return fabsf(val - roundf(val));
}

void PlayerController::tick(float deltaTime) {
    camera.tick(deltaTime);

    //Block breaking
    if (InputManager::mouse1Pressed){
        glm::vec3 objcoord = getLookingBlock(false);
        if (objcoord.x==NAN)return;

        int x = floorf(objcoord.x);
        int y = floorf(objcoord.y);
        int z = floorf(objcoord.z);

        if (std::abs(camera.getCameraPos().x-x)<PLAYER_REACH && std::abs(camera.getCameraPos().y-y)<PLAYER_REACH && std::abs(camera.getCameraPos().z-z)<PLAYER_REACH){
            superchunk.set(x, y, z, 0);
        }
    }

    //Block placing
    if (InputManager::mouse2Pressed){
        glm::vec3 objcoord = getLookingBlock(true);
        if (objcoord.x==NAN)return;

        int x = floorf(objcoord.x);
        int y = floorf(objcoord.y);
        int z = floorf(objcoord.z);

        if (std::abs(camera.getCameraPos().x-x)<PLAYER_REACH && std::abs(camera.getCameraPos().y-y)<PLAYER_REACH && std::abs(camera.getCameraPos().z-z)<PLAYER_REACH){
            glm::vec3 lookDir = camera.lookDirection();
            lookDir*=0.1;
            glm::vec3 actualBlock = objcoord+lookDir;

            int ox = floorf(actualBlock.x);
            int oy = floorf(actualBlock.y);
            int oz = floorf(actualBlock.z);

            if (x!=ox){
                superchunk.set(x, oy, oz, Block::getBlockById(1));
            }else if(y!=oy){
                superchunk.set(ox, y, oz, Block::getBlockById(1));
            }else{
                superchunk.set(ox, oy, z, Block::getBlockById(1));
            }
        }
    }
}

glm::vec3 PlayerController::getLookingBlock(bool blockBefore) {
    //  Block exist at (x_0, y_0, z_0): superchunk.get(x,y,z) != 0
    //  Camera position: camera.getCameraPos()
    //  View & Projection matrix: camera.getView() camera.getProj()
    //  Looking direction unit vector (glm::vec3): camera.lookDirection()

    glm::vec3 cameraPos = camera.getCameraPos();
    glm::vec3 direction = camera.lookDirection();
    direction*=0.1;

    while (glm::length(cameraPos-camera.getCameraPos())<PLAYER_REACH){
        cameraPos+=direction;
        if (superchunk.get(floor(cameraPos.x), floor(cameraPos.y), floor(cameraPos.z))){
            if (blockBefore){
                return glm::vec3{cameraPos.x-direction.x, cameraPos.y-direction.y, cameraPos.z-direction.z};
            }else{
                return glm::vec3{cameraPos.x, cameraPos.y, cameraPos.z};
            }
        }
    }
    return glm::vec3(NAN, NAN, NAN);

//    //  Step 1: set origin to camera position
//    glm::vec3 r_o = camera.getCameraPos();
//    const glm::vec3 origin = r_o;
//    //  Step 2: get the looking direction
//    glm::vec3 u = glm::normalize(camera.lookDirection());
//
//    //  Step 3: loop until you're too far
//    auto distance = 0.0;
//    while (glm::length(r_o - origin) < PLAYER_REACH) {
//        //  Step 4: calculate how far is the next possible block
//        double dt_x = (trunc(r_o.x) + 1.0 - r_o.x) / u.x != 0 ?: 2.0;
//        double dt_y = (trunc(r_o.y) + 1.0 - r_o.y) / u.y != 0 ?: 2.0;
//        double dt_z = (trunc(r_o.z) + 1.0 - r_o.z) / u.z != 0 ?: 2.0;
//        //  Step 5: check which is the closest
//        enum : uint8_t
//        {
//            X_SMALLEST,
//            y_SMALLEST,
//            z_SMALLEST
//        };
//        uint8_t option = dt_x < dt_y;
//        double block_x, block_y, block_z;
//        option = - option * (dt_x < dt_z) + (1 - option) * (dt_y < dt_z);
//        assert(option == 0 || option == 1 || option == -1);
//        switch (option)
//        {
//            case -1:    //  X is the smallest
//                r_o = {r_o.x + u.x * dt_x, r_o.y + u.y * dt_x, r_o.y + u.y * dt_x};
//                block_x = r_o.x;
//                block_y = floor(r_o.y);
//                block_z = floor(r_o.z);
//                break;
//
//            case 1:     //  Y is the smallest
//                r_o = {r_o.x + u.x * dt_y, r_o.y + u.y * dt_y, r_o.y + u.y * dt_y};
//                block_x = floor(r_o.x);
//                block_y = r_o.y;
//                block_z = floor(r_o.z);
//                break;
//
//            case 0:     //  Z is the smallest
//                r_o = {r_o.x + u.x * dt_z, r_o.y + u.y * dt_z, r_o.y + u.y * dt_z};
//                block_x = floor(r_o.x);
//                block_y = floor(r_o.y);
//                block_z = r_o.z;
//                break;
//        }
//        if (block_x < 0.0 || block_y < 0.0 || block_z < 0.0)
//        {
//            continue;
//        }
//
//        if (superchunk.get(block_x, block_y, block_z) != 0)
//        {
//            return glm::vec3{block_x, block_y, block_z};
//        }
//    }
//    return glm::vec3{NAN, NAN, NAN};


//    GLfloat depth;
//    glReadPixels(window.getWidth() / 2, window.getHeight() / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
//
//    glm::vec4 viewport = glm::vec4(0, 0, window.getWidth(), window.getHeight());
//    glm::vec3 wincoord = glm::vec3(window.getWidth() / 2, window.getHeight() / 2, depth);
//    glm::vec3 objcoord = glm::unProject(wincoord, camera.getView(), camera.getProj(), viewport);
//
//    return objcoord;
}

PlayerController::PlayerController(Camera &camera, Superchunk &superchunk, Window &window) : camera(camera),
                                                                                             superchunk(superchunk),
                                                                                             window(window) {}
