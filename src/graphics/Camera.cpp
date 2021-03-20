//
// Created by quartzy on 2/9/21.
//

#include "Camera.h"
#include <GLFW/glfw3.h>

#include <cmath>

void Camera::tick(float deltaTime) {
    double xpos, ypos;
    glfwGetCursorPos(windowHandle, &xpos, &ypos);

    glfwSetCursorPos(windowHandle, windowWidth/2.0, windowHeight/2.0);

    horizontalAngle += mouseSpeed * deltaTime * float(windowWidth/2.0 - xpos );
    verticalAngle   += mouseSpeed * deltaTime * float( windowHeight/2.0 - ypos );

    glm::vec3 direction(
            std::cos(verticalAngle) * std::sin(horizontalAngle),
            std::sin(verticalAngle),
            std::cos(verticalAngle) * std::cos(horizontalAngle)
    );

    glm::vec3 moveDirection(
            std::sin(horizontalAngle),
            0,
            std::cos(horizontalAngle)
            );

    glm::vec3 right = glm::vec3(
            std::sin(horizontalAngle - 3.14f/2.0f),
            0,
            std::cos(horizontalAngle - 3.14f/2.0f)
    );

    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey(windowHandle, GLFW_KEY_W ) == GLFW_PRESS){
        cameraPos += moveDirection * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(windowHandle, GLFW_KEY_S ) == GLFW_PRESS){
        cameraPos -= moveDirection * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(windowHandle, GLFW_KEY_D ) == GLFW_PRESS){
        cameraPos += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(windowHandle, GLFW_KEY_A ) == GLFW_PRESS){
        cameraPos -= right * deltaTime * speed;
    }

    if (glfwGetKey(windowHandle, GLFW_KEY_SPACE ) == GLFW_PRESS){
        cameraPos += glm::vec3(0, 1, 0) * deltaTime * speed;
    }

    if (glfwGetKey(windowHandle, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
        cameraPos -= glm::vec3(0, 1, 0) * deltaTime * speed;
    }

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

float Camera::getMouseSpeed() const {
    return mouseSpeed;
}

void Camera::setMouseSpeed(float mouseSpeed) {
    Camera::mouseSpeed = mouseSpeed;
}

float Camera::getSpeed() const {
    return speed;
}

void Camera::setSpeed(float speed) {
    Camera::speed = speed;
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
    return glm::vec3(
            std::cos(verticalAngle) * std::sin(horizontalAngle),
            std::sin(verticalAngle),
            std::cos(verticalAngle) * std::cos(horizontalAngle)
    );
}
