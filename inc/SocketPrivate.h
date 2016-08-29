#ifndef SOCKET_PRIVATE_INCLUDED
#define SOCKET_PRIVATE_INCLUDED

#include "Socket.h"

typedef struct SocketStruct
{
    int file_descriptor;
    const char * ip_address;
    int port;
} SocketStruct;

#endif
