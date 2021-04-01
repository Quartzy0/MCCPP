//
// Created by quartzy on 2/9/21.
//

#include "Camera.h"
#include <GLFW/glfw3.h>

#include <cmath>

void Camera::tick(float deltaTime) {
    direction = glm::vec3{
            std::cos(verticalAngle) * std::sin(horizontalAngle),
            std::sin(verticalAngle),
            std::cos(verticalAngle) * std::cos(horizontalAngle)
    };

    moveDirection = glm::vec3{
            std::sin(horizontalAngle),
            0,
            std::cos(horizontalAngle)
    };

    right = glm::vec3{
            std::sin(horizontalAngle - 3.14f/2.0f),
            0,
            std::cos(horizontalAngle - 3.14f/2.0f)
    };

    up = glm::cross( right, direction );

    proj = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);
// Camera matrix
    view = glm::lookAt(
            cameraPos,           // Camera is here
            cameraPos+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    vp = proj * view;
}

Camera::Camera(GLint windowWidth, GLint windowHeight) : windowWidth(windowWidth),
                                                        windowHeight(windowHeight),
                                                        cameraPos(0, 0, 2){

}

void Camera::init(GLFWwindow *windowHandle) {
    Camera::windowHandle = windowHandle;
}

const glm::mat4 &Camera::getVp() const {
    return vp;
}

const glm::mat4 &Camera::getProj() const {
    return proj;
}

const glm::mat4 &Camera::getView() const {
    return view;
}

const glm::vec3 &Camera::getCameraPos() const {
    return cameraPos;
}

float Camera::getHorizontalAngle() const {
    return horizontalAngle;
}

float Camera::getVerticalAngle() const {
    return verticalAngle;
}

float Camera::getFov() const {
    return fov;
}

glm::vec3 Camera::lookDirection() {
    return direction;
}

void Camera::setCameraPos(glm::vec3 cameraPos) {
    Camera::cameraPos = cameraPos;
}

void Camera::setHorizontalAngle(float horizontalAngle) {
    Camera::horizontalAngle = horizontalAngle;
}

void Camera::setVerticalAngle(float verticalAngle) {
    Camera::verticalAngle = verticalAngle;
}

GLFWwindow *Camera::getWindowHandle() const {
    return windowHandle;
}

const glm::vec3 &Camera::getRight() const {
    return right;
}

const glm::vec3 &Camera::getUp() const {
    return up;
}

const glm::vec3 &Camera::getMoveDirection() const {
    return moveDirection;
}

const glm::vec3 &Camera::getDirection() const {
    return direction;
}

GLint Camera::getWindowWidth() const {
    return windowWidth;
}

GLint Camera::getWindowHeight() const {
    return windowHeight;
}
