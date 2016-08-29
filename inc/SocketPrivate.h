#ifndef SOCKET_PRIVATE_INCLUDED
#define SOCKET_PRIVATE_INCLUDED

#include "Socket.h"

typedef struct SocketInterfaceStruct * SocketInterface;

// ? Might want to add an options void pointer.
// Will eventually need to return a struct, probably.
typedef int (*SocketOpen)(void);
// Will eventually need to accept a file descriptor struct, probably.
typedef void (*SocketClose)(int);
// Will eventually need to accept a file descriptor struct, probably.
typedef int (*SocketSend)(int file_descriptor, const char * message, unsigned int message_length);
typedef struct SocketInterfaceStruct
{
    SocketOpen open;
    SocketClose close;
    SocketSend send;
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
