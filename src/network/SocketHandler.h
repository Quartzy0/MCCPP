//
// Created by quartzy on 2/23/21.
//

#ifndef TESTING1_SOCKETHANDLER_H
#define TESTING1_SOCKETHANDLER_H

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501  /* Windows XP. */
    #endif
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    typedef unsigned int Socket;

#else
    /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
    #include <unistd.h> /* Needed for close() */

    typedef int Socket;

#endif


class SocketHandler {

    int sockInit();

    int sockQuit();

    int sockClose(Socket sock);
};


#endif //TESTING1_SOCKETHANDLER_H
