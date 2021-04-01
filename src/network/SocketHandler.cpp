//
// Created by quartzy on 2/23/21.
//

#include "SocketHandler.h"

int SocketHandler::sockInit() {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
#else
    return 0;
#endif
}

int SocketHandler::sockQuit() {
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}

int SocketHandler::sockClose(Socket sock) {
    int status = 0;

#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0) { status = closesocket(sock); }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) { status = close(sock); }
#endif

    return status;
}
