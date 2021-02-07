//
// Created by quartzy on 2/6/21.
//

#ifndef MCCPP_WINDOW_H
#define MCCPP_WINDOW_H


#include <string>
#include <GLFW/glfw3.h>

class Window {
public:
    int init();

    Window(std::string title, uint32_t width, uint32_t height);

    [[nodiscard]] const std::string &getTitle() const;

    [[nodiscard]] uint32_t getWidth() const;

    [[nodiscard]] uint32_t getHeight() const;

    [[nodiscard]] GLFWwindow *getWindow() const;

private:
    std::string title;
    uint32_t width, height;
    GLFWwindow* window = nullptr;
};


#endif //MCCPP_WINDOW_H
