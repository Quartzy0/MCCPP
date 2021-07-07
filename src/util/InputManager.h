//
// Created by quartzy on 2/20/21.
//

#ifndef MCCPP_INPUTMANAGER_H
#define MCCPP_INPUTMANAGER_H

#include <cstdint>
#include <vector>
#include <GLFW/glfw3.h>

struct Keybind{
    uint32_t keycode;
    //Pressed THIS frame
    bool pressed;
    bool prevPressed;
    //Is key being held down
    bool keyDown;
};

class InputManager {
private:
    static std::vector<Keybind> keybinds;
    static GLFWwindow* window;

    static bool mouse1Pressed_prev;
    static bool mouse2Pressed_prev;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
    static void init(GLFWwindow* window);

    //Call this function LAST
    static void tick();

    static void setCursorEnabled(bool enabled);

    static Keybind & addKeybind(uint32_t keybind);

    static bool mouse1Pressed;
    static bool mouse2Pressed;
};


#endif //MCCPP_INPUTMANAGER_H
