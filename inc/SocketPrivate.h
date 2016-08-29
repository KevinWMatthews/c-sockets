#ifndef SOCKET_PRIVATE_INCLUDED
#define SOCKET_PRIVATE_INCLUDED

#include "Socket.h"

/*
 * All wrappers modules must implement this interface.
 */
// Call this on the base Socket immediately after creating any socket.
void Socket_Initialize(Socket);

/*
 * Client and server calls
 */
// Returns a pointer to a new Socket object, or a null pointer on failure.
typedef Socket (*SocketCreate)(void);

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
    SocketCreate create;
    SocketOpen open;
    SocketClose close;
    SocketSend send;
    SocketReceive receive;
    SocketConnect connect;
    SocketBind bind;
    SocketListen listen;
    SocketAccept accept;
} SocketInterfaceStruct;

typedef struct SocketStruct
{
    SocketInterface interface;
    int file_descriptor;
    const char * ip_address;
    int port;
} SocketStruct;

#endif
