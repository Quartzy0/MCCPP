cmake_minimum_required(VERSION 3.17)
project(MCCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(MCCPP src/main.cpp src/graphics/Window.cpp src/graphics/Window.h src/graphics/ShaderProgram.cpp src/graphics/ShaderProgram.h src/graphics/buffers/VertexBuffer.cpp src/graphics/buffers/VertexBuffer.h)
target_compile_options(MCCPP PRIVATE -Wall)

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