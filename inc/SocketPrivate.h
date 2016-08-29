#ifndef SOCKET_PRIVATE_INCLUDED
#define SOCKET_PRIVATE_INCLUDED

#include "Socket.h"

// ? Might want to add an options void pointer.
// Will eventually need to change the file descriptor from an in to a struct, probably.
typedef int (*SocketOpen)(void);
typedef void (*SocketClose)(int);
typedef int (*SocketSend)(int file_descriptor, const char * message, unsigned int message_length);
typedef int (*SocketReceive)(int file_descriptor, char * buffer, unsigned int buffer_length);
typedef int (*SocketConnect)(int file_descriptor, const char * ip_address, int port);
typedef int (*SocketBind)(int file_descriptor, const char * ip_address, int port);
typedef int (*SocketAccept)(int file_descriptor);
typedef int (*SocketListen)(int file_descriptor, int backlog);
typedef struct SocketInterfaceStruct
{
    SocketOpen open;
    SocketClose close;
    SocketSend send;
    SocketReceive receive;
    SocketConnect connect;
    SocketBind bind;
    SocketAccept accept;
    SocketListen listen;
} SocketInterfaceStruct;

typedef struct SocketStruct
{
    SocketInterface interface;
    SocketOpen open;
    int file_descriptor;
    const char * ip_address;
    int port;
} SocketStruct;

#endif
