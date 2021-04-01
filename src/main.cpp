#include <iostream>
#include <GL/glew.h>

#include "MCCPP.h"
#include "PlayerController.h"
#include "TextureAtlas.h"
#include "graphics/Vertex.h"
#include "util/InputManager.h"
#include "util/Log.h"
#include "network/NetworkManager.h"
#include "network/packet/login/PacketHandshake.h"
#include "network/packet/login/PacketLoginStart.h"
#include "network/HTTPRequest.hpp"

#include <glm/gtc/noise.hpp>

using namespace network;

bool paused = false;

double avgFrameTime;
uint32_t framesPerSecond;

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam){
    if (severity==GL_DEBUG_SEVERITY_NOTIFICATION)return;
    printf("%s\n", message);
}

int to_int(char const *s)
{
    if ( s == NULL || *s == '\0' )
        throw std::invalid_argument("null or empty string argument");

    bool negate = (s[0] == '-');
    if ( *s == '+' || *s == '-' )
        ++s;

    if ( *s == '\0')
        throw std::invalid_argument("sign character only.");

    int result = 0;
    while(*s)
    {
        if ( *s < '0' || *s > '9' )
            throw std::invalid_argument("invalid input string");
        result = result * 10  - (*s - '0');  //assume negative number
        ++s;
    }
    return negate ? result : -result; //-result is positive!
}

int main(int argc, char** argv) {
    try{
        Log::init();

//        if (argc>=5){
//            CORE_ERROR("Need 5 arguments: [Username] [UUID] [AuthToken] [ServerIP] [ServerPort]");
//            exit(1);
//        }
        MCCPP::username = std::string{argv[1]};
        MCCPP::uuid = std::string{argv[2]};
        MCCPP::authToken = std::string{argv[3]};
        MCCPP::serverIP = std::string{argv[4]};
        MCCPP::port = to_int(argv[5]);


        Window window = Window("Test", 1280, 720);
        ShaderProgram shaderProgram = ShaderProgram("res/shaders/vertex.glsl", "res/shaders/geometry.glsl", "res/shaders/fragment.glsl");

        shaderProgram.addVertexAttrib(0, 3, GL_BYTE, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,x));
        shaderProgram.addVertexAttrib(1, 1, GL_SHORT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex,textureId));

        window.init();
        shaderProgram.init();

        Camera camera(window.getWidth(), window.getHeight());
        camera.init(window.getWindow());

        InputManager::init(window.getWindow());
//        InputManager::setCursorEnabled(false);

        TextureAtlas textureAtlas;
        textureAtlas.load("res/textures/blocks", 64);

        Superchunk superchunk(shaderProgram);

        PlayerController playerController{camera, superchunk, window};

        MCCPP::playerController = &playerController;
        MCCPP::superchunk = &superchunk;

        Block::initBlocks(textureAtlas);

        Block* stone = Block::getBlockById(1);
        Block* dirt = Block::getBlockById(56);
//        superchunk.set(2, 0, 0, stone);
//        superchunk.set(3, 0, 0, dirt);

        for (int k = 0; k < 10; ++k) {
            for (int j = 0; j < 10; ++j) {
                for (int i = 0; i < 10; ++i) {
                    superchunk.set(i-236, k+32, j+256, stone);
                }
            }
        }

//        All chunks
//        for (int chunkX = -(SCX/2); chunkX < SCX/2; ++chunkX) {
//            for (int chunkZ = -(SCZ/2); chunkZ < SCZ/2; ++chunkZ) {
//                //chunk perlin noise
//                for (int x = chunkX*CX; x < chunkX*CX+CX; ++x) {
//                    for (int z = chunkZ*CZ; z < chunkZ*CZ+CZ; ++z) {
//                        int32_t yLevel = glm::roundEven((glm::perlin( glm::vec2(x / 8., z / 8. ) ) + 1.0f) * 20 );
//                        for (int y = 0; y < yLevel; ++y) {
//                            if (y>=yLevel-4){
//                                superchunk.set(x-236, y+32, z+256, dirt);
//                            }else{
//                                superchunk.set(x-236, y+32, z+256, stone);
//                            }
//                        }
//                    }
//                }
//            }
//        }

        shaderProgram.bind();

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDebugMessageCallback(debug_callback, nullptr);

        CORE_INFO("Initialized successfully");


        CORE_INFO("Attempting to connect to {}:{}", MCCPP::serverIP, MCCPP::port);
        http::init();
        NetworkManager::init(MCCPP::serverIP.c_str(), MCCPP::port);

        NetworkManager::setState(State::HANDSHAKE);

        PacketHandshake packetHandshake{47, MCCPP::serverIP, MCCPP::port, 2};
        NetworkManager::sendPacket(packetHandshake);
        NetworkManager::setState(State::LOGIN);

        PacketLoginStart packetLoginStart{MCCPP::username};
        NetworkManager::sendPacket(packetLoginStart);



        double lastTime = glfwGetTime();
        double lastTickTime = -1;

        double lastSecond = glfwGetTime();
        double timeSpent = 0;
        double longestFrame = 0;

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
            shaderProgram.setUniform("lightLevel", 1);
            superchunk.render(vp);

            InputManager::tick();

            MCCPP::tick(deltaTime);

            lastTime = glfwGetTime();

            framesThisSecond++;
            double newTime = glfwGetTime()-currentTime;
            timeSpent+=newTime;
            if (newTime>longestFrame){
                longestFrame = newTime;
            }

            if (glfwGetTime()-lastSecond>=1.0){
                framesPerSecond = framesThisSecond;
                avgFrameTime = (timeSpent/framesThisSecond)*1000.0;
                CORE_INFO("{} FPS, {} ms avg. frame time, {} ms total time spent. Longest was {} ms", framesPerSecond, avgFrameTime, timeSpent*1000, longestFrame*1000);
                CORE_INFO("In chunk {} {} {}", std::trunc(MCCPP::playerController->getPos().x/16), std::trunc(MCCPP::playerController->getPos().y/16), std::trunc(MCCPP::playerController->getPos().z/16));
                timeSpent = 0;
                framesThisSecond = 0;
                longestFrame = 0;
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

        NetworkManager::cleanup();
        http::cleanup();

        glfwTerminate();
    }catch (std::exception& e){
        std::cerr << e.what() << "\n";
    }
    return 0;
}
