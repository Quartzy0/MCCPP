cmake_minimum_required(VERSION 3.17)
project(MCCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(MCCPP src/main.cpp src/graphics/Window.cpp src/graphics/Window.h src/graphics/ShaderProgram.cpp src/graphics/ShaderProgram.h src/util/Log.cpp src/util/Log.h src/MCCPP.h src/graphics/Camera.cpp src/graphics/Camera.h src/graphics/Vertex.h src/lodepng/lodepng.cpp src/lodepng/lodepng.h src/blocks/Chunk.cpp src/blocks/Chunk.h src/blocks/Superchunk.cpp src/blocks/Superchunk.h src/TextureAtlas.cpp src/TextureAtlas.h src/util/InputManager.cpp src/util/InputManager.h src/PlayerController.cpp src/PlayerController.h src/blocks/Block.cpp src/blocks/Block.h src/network/NetworkManager.cpp src/network/NetworkManager.h src/network/HTTPRequest.hpp src/network/SocketHandler.h src/network/SocketHandler.cpp src/network/Encryption.hpp src/network/digest.h src/network/hashlib.h
        src/network/packet/login/PacketHandshake.cpp src/network/packet/login/PacketHandshake.h src/network/packet/login/PacketLoginStart.cpp src/network/packet/login/PacketLoginStart.h src/network/packet/login/PacketEncryptionRequest.cpp src/network/packet/login/PacketEncryptionRequest.h src/network/packet/login/PacketEncryptionResponse.cpp src/network/packet/login/PacketEncryptionResponse.h src/network/packet/login/PacketSetCompression.cpp src/network/packet/login/PacketSetCompression.h src/network/packet/Packet.cpp src/network/packet/login/PacketLoginSuccess.cpp src/network/packet/login/PacketLoginSuccess.h src/network/packet/login/PacketDisconnect.cpp src/network/packet/login/PacketDisconnect.h src/network/packet/play/PacketJoinGame.cpp src/network/packet/play/PacketJoinGame.h src/MCCPP.cpp src/network/packet/play/PacketPluginMessage.cpp src/network/packet/play/PacketPluginMessage.h src/network/packet/play/PacketKeepAlive.cpp src/network/packet/play/PacketKeepAlive.h src/network/packet/play/PacketServerDifficulty.cpp src/network/packet/play/PacketServerDifficulty.h src/network/packet/play/PacketSpawnPosition.cpp src/network/packet/play/PacketSpawnPosition.h src/network/packet/play/PacketHeldItemChange.cpp src/network/packet/play/PacketHeldItemChange.h src/network/packet/play/PacketPlayerListItem.cpp src/network/packet/play/PacketPlayerListItem.h src/PlayerMP.cpp src/PlayerMP.h src/util/UUID.h src/network/packet/play/PacketPlayerAbilities.cpp src/network/packet/play/PacketPlayerAbilities.h src/network/packet/play/PacketPlayerPosAndLook.cpp src/network/packet/play/PacketPlayerPosAndLook.h src/network/packet/play/PacketStatistics.cpp src/network/packet/play/PacketStatistics.h src/network/packet/play/PacketTimeUpdate.cpp src/network/packet/play/PacketTimeUpdate.h src/network/packet/play/PacketMapChunkBulk.cpp src/network/packet/play/PacketMapChunkBulk.h)
target_compile_options(MCCPP PRIVATE -Wall)

#OpenSSL
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)

#cURL
set(CURL_LIBRARY "-lcurl -lpthread")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(MCCPP PRIVATE ${CURL_LIBRARIES})

#ZLib
find_package(ZLIB REQUIRED)
target_link_libraries(MCCPP PRIVATE ${ZLIB_LIBRARIES})

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