//
// Created by quartzy on 2/6/21.
//
#include <GL/glew.h>

#include "Window.h"

#include <utility>

int Window::init() {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        return -1;
    }
    return 0;
}

Window::Window(std::string title, uint32_t width, uint32_t height):
    title(std::move(title)), width(width), height(height){}

const std::string &Window::getTitle() const {
    return title;
}

uint32_t Window::getWidth() const {
    return width;
}

uint32_t Window::getHeight() const {
    return height;
}

GLFWwindow *Window::getWindow() const {
    return window;
}