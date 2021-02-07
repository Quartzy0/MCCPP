#include <iostream>
#include <GL/glew.h>

#include "graphics/Window.h"
#include "graphics/ShaderProgram.h"
#include "graphics/buffers/VertexBuffer.h"

void render(Window& window, VertexBuffer& vertexBuffer){

    vertexBuffer.bind();
    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void tick(){

}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam){
    printf("%s\n", message);
}

int main() {
    Window window = Window("Hello warudo", 800, 600);
    std::cout << "Window created: " << window.init() << "\n";
    ShaderProgram shaderProgram = ShaderProgram("", "");
    shaderProgram.init();

    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(debug_callback, nullptr);

    GLfloat triangle_vertices[] = {
            0.0,  0.8,
            -0.8, -0.8,
            0.8, -0.8,
    };

    VertexBuffer buffer = VertexBuffer();
    buffer.bind();
    buffer.uploadData(triangle_vertices, 6* sizeof(float), GL_STATIC_DRAW);

    GLint int1 = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &int1);

    glUseProgram(shaderProgram.getProgram());
    glBindVertexArray(0);
    glEnableVertexAttribArray(shaderProgram.getAttributeCoord2D());
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(shaderProgram.getAttributeCoord2D(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


    std::cout << "Initialized successfully\n";

    while (!glfwWindowShouldClose(window.getWindow()))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        tick();
        render(window, buffer);

        /* Swap front and back buffers */
        glfwSwapBuffers(window.getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
