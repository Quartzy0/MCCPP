//
// Created by quartzy on 2/20/21.
//

#include "InputManager.h"
#include "Log.h"

std::vector<Keybind> InputManager::keybinds;
GLFWwindow* InputManager::window;
bool InputManager::mouse1Pressed;
bool InputManager::mouse2Pressed;
bool InputManager::mouse1Pressed_prev;
bool InputManager::mouse2Pressed_prev;

void InputManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (Keybind& keybind : keybinds){
        if (key == keybind.keycode && action == GLFW_PRESS){
            keybind.keyDown = true;
            CORE_TRACE("Key {} pressed", key);
        }
        if (key == keybind.keycode && action == GLFW_RELEASE){
            keybind.keyDown = false;
            CORE_TRACE("Key {} released", key);
        }
    }
}

void InputManager::tick() {
    for(Keybind& keybind : keybinds){
        keybind.pressed = keybind.keyDown && !keybind.prevPressed;
        keybind.prevPressed = keybind.keyDown;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)==GLFW_PRESS && !mouse1Pressed_prev){
        mouse1Pressed = true;
    }else{
        mouse1Pressed = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)==GLFW_PRESS && !mouse2Pressed_prev){
        mouse2Pressed = true;
    }else{
        mouse2Pressed = false;
    }
    mouse2Pressed_prev = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)==GLFW_PRESS;
    mouse1Pressed_prev = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)==GLFW_PRESS;
}

void InputManager::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    InputManager::window = window;
}

void InputManager::setCursorEnabled(bool enabled) {
    glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

Keybind & InputManager::addKeybind(uint32_t keybind) {
    Keybind keybind1{keybind, false, false, false};
    return keybinds.emplace_back(keybind1);
}
