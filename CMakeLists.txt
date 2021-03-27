cmake_minimum_required(VERSION 3.17)
project(MCCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(MCCPP src/main.cpp src/graphics/Window.cpp src/graphics/Window.h src/graphics/ShaderProgram.cpp src/graphics/ShaderProgram.h src/util/Log.cpp src/util/Log.h src/MCCPP.h src/graphics/Camera.cpp src/graphics/Camera.h src/graphics/Vertex.h src/lodepng/lodepng.cpp src/lodepng/lodepng.h src/blocks/Chunk.cpp src/blocks/Chunk.h src/blocks/Superchunk.cpp src/blocks/Superchunk.h src/TextureAtlas.cpp src/TextureAtlas.h src/util/InputManager.cpp src/util/InputManager.h src/PlayerController.cpp src/PlayerController.h src/blocks/Block.cpp src/blocks/Block.h)
target_compile_options(MCCPP PRIVATE -Wall)

#tbb
target_link_libraries(MCCPP PRIVATE tbb)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(MCCPP PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(MCCPP PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(MCCPP PRIVATE glm)

# spdlog
add_subdirectory(lib/spdlog EXCLUDE_FROM_ALL)
target_link_libraries(MCCPP PRIVATE spdlog)

#json
include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(MCCPP PRIVATE nlohmann_json::nlohmann_json)