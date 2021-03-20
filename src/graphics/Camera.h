//
// Created by quartzy on 2/9/21.
//

#ifndef MCCPP_CAMERA_H
#define MCCPP_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"

class Camera {
private:
    //Matrices
    glm::mat4 proj{};
    glm::mat4 view{};
    glm::mat4 vp{};

    //Position
    glm::vec3 cameraPos;

    //Rotation
    float horizontalAngle = 3.14f; //Horizontal angle: toward -Z
    float verticalAngle = 0.0f; //Vertical angle: 0, look at the horizon
    float fov = 90.0f; //Initial Field of View

    float mouseSpeed = 0.2f;
    float speed = 10;

    GLint windowWidth, windowHeight;
    GLFWwindow* windowHandle{};
public:
    Camera(GLint windowWidth, GLint windowHeight);

    void init(GLFWwindow* windowHandle);

    void tick(float deltaTime);

    float getMouseSpeed() const;

    void setMouseSpeed(float mouseSpeed);

    float getSpeed() const;

    void setSpeed(float speed);

    const glm::mat4 &getProj() const;

    const glm::mat4 &getView() const;

    [[nodiscard]] const glm::mat4 &getVp() const;

    const glm::vec3 &getCameraPos() const;

    float getHorizontalAngle() const;

    float getVerticalAngle() const;

    float getFov() const;

    glm::vec3 lookDirection();
};


#endif //MCCPP_CAMERA_H
