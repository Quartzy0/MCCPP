#include <iostream>
#include <GL/glew.h>

#include "MCCPP.h"
#include "PlayerController.h"
#include "TextureAtlas.h"
#include "graphics/Vertex.h"
#include "util/InputManager.h"

#include <glm/gtc/noise.hpp>

using namespace MCCPP;

bool paused = false;

double avgFrameTime;
uint32_t framesPerSecond;

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam){
    if (severity==GL_DEBUG_SEVERITY_NOTIFICATION)return;
    printf("%s\n", message);
}

int main() {
    try{
        Log::init();
        CORE_ERROR("HELLO!");

        Window window = Window("Test", 1280, 720);
        ShaderProgram shaderProgram = ShaderProgram("res/shaders/vertex.glsl", "res/shaders/geometry.glsl", "res/shaders/fragment.glsl");

        shaderProgram.addVertexAttrib(0, 3, GL_BYTE, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,x));
        shaderProgram.addVertexAttrib(1, 1, GL_SHORT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,textureId));

        window.init();
        shaderProgram.init(16);

        Camera camera(window.getWidth(), window.getHeight());
        camera.init(window.getWindow());

        InputManager::init(window.getWindow());
//        InputManager::setCursorEnabled(false);

        TextureAtlas textureAtlas;
        textureAtlas.load("res/textures/blocks", 64);

        Superchunk superchunk(shaderProgram);

        PlayerController playerController{camera, superchunk, window};

        Block::initBlocks(textureAtlas);

        Block* stone = Block::getBlockById(1);
        Block* dirt = Block::getBlockById(56);
//        superchunk.set(2, 0, 0, stone);
//        superchunk.set(3, 0, 0, dirt);

//        for (int k = 0; k < 10; ++k) {
//            for (int j = 0; j < 10; ++j) {
//                for (int i = 0; i < 10; ++i) {
//                    superchunk.set(i+10, k, j, stone);
//                }
//            }
//        }

        double startTime = glfwGetTime();
//        All chunks
        for (int chunkX = -(SCX/2); chunkX < SCX/2; ++chunkX) {
            for (int chunkZ = -(SCZ/2); chunkZ < SCZ/2; ++chunkZ) {
                //chunk perlin noise
                for (int x = chunkX*CX; x < chunkX*CX+CX; ++x) {
                    for (int z = chunkZ*CZ; z < chunkZ*CZ+CZ; ++z) {
                        int32_t yLevel = glm::roundEven((glm::perlin( glm::vec2(x / 8., z / 8. ) ) + 1.0f) * 20 );
                        for (int y = 0; y < yLevel; ++y) {
                            if (y>=yLevel-4){
                                superchunk.set(x, y, z, dirt);
                            }else{
                                superchunk.set(x, y, z, stone);
                            }
                        }
                    }
                }
            }
        }

        double timeTaken = glfwGetTime()-startTime;
        std::cout << "Took " << timeTaken << "s\n";

        shaderProgram.bind();

        PointLight lights[16];
        lights[0] = PointLight{glm::vec3{1, 1, 1}, glm::vec3{10, 100, 0}, 1, 1, 0.0008, 0.0001};

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDebugMessageCallback(debug_callback, nullptr);

        CORE_INFO("Initialized successfully");

        double lastTime = glfwGetTime();
        double lastTickTime = -1;

        double lastSecond = glfwGetTime();
        double timeSpent = 0;

        uint32_t framesThisSecond = 0;

        uint32_t mcTick = 1;

        while (!glfwWindowShouldClose(window.getWindow()))
        {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.2, 0.2, 0.2, 1);

            if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE)){
                paused = !paused;
            }

            textureAtlas.tick(mcTick);

            glm::mat4 vp = camera.getVp();
            shaderProgram.bind();
            shaderProgram.setUniform("textureArray", 0);
            shaderProgram.setPointLights(lights, 1);
            superchunk.render(vp);

            if (!paused){
                playerController.tick(deltaTime);
            }

            InputManager::tick();

            lastTime = glfwGetTime();

            framesThisSecond++;
            timeSpent+=glfwGetTime()-currentTime;

            if (glfwGetTime()-lastSecond>=1.0){
                framesPerSecond = framesThisSecond;
                avgFrameTime = (timeSpent/framesThisSecond)*1000.0;
                CORE_INFO("{} FPS, {} ms avg. frame time, {} ms total time spent", framesPerSecond, avgFrameTime, timeSpent*1000);
                timeSpent = 0;
                framesThisSecond = 0;
                lastSecond = glfwGetTime();
            }

            if (lastTickTime==-1 || glfwGetTime()-lastTickTime>=1.0/20.0){
                lastTickTime = glfwGetTime();
                mcTick++;
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window.getWindow());

            /* Poll for and process events */
            glfwPollEvents();
        }



        glfwTerminate();
    }catch (std::exception& e){
        std::cerr << e.what() << "\n";
    }
    return 0;
}
