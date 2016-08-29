#ifndef SOCKET_PRIVATE_INCLUDED
#define SOCKET_PRIVATE_INCLUDED

#include "Socket.h"

/*
 * All wrappers modules must implement this interface.
 */

/*
 * Client and server calls
 */
// Return >= 0 file descriptor on success, < 0 on failure.
typedef int (*SocketOpen)(void);

typedef void (*SocketClose)(int file_descriptor);

// Returns the number of bytes sent, or < 0 on failure.
typedef int (*SocketSend)(int file_descriptor, const char * message, unsigned int message_length);

// Returns the number of bytes received, or < 0 on failure. 
typedef int (*SocketReceive)(int file_descriptor, char * buffer, unsigned int buffer_length);

/*
 * Client calls
 */
// Return 0 on success, < 0 on failure.
typedef int (*SocketConnect)(int file_descriptor, const char * ip_address, int port);

/*
 * Server calls
 */
// Return 0 on success, < 0 on failure.
typedef int (*SocketBind)(int file_descriptor, const char * ip_address, int port);

// Return 0 on success, < 0 on failure.
typedef int (*SocketListen)(int file_descriptor, int backlog);

// Return 0 on success, < 0 on failure.
typedef int (*SocketAccept)(int file_descriptor);

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
