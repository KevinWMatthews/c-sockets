#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;
typedef struct SocketInterfaceStruct * SocketInterface;

void Socket_Destroy(Socket *);

typedef enum
{
    SOCKET_INVALID_PORT = -3,
    SOCKET_NULL_POINTER = -2,
    SOCKET_FAIL = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;
int Socket_Open(Socket);
void Socket_Close(Socket);

typedef enum
{
    SOCKET_INVALID_FILE_DESCRIPTOR = -1
} SocketFileDescriptor;
int Socket_GetFileDescriptor(Socket);
const char * Socket_GetIpAddress(Socket);
int Socket_GetPort(Socket);

int Socket_Send(Socket, const char * message, unsigned int message_length);
int Socket_Receive(Socket, char * buffer, unsigned int buffer_length);

// Client only.
int Socket_Connect(Socket, const char * ip_address, int port);

// Server only.
int Socket_Bind(Socket, const char * ip_address, int port);
int Socket_Listen(Socket, int backlog);
// Returns a handle to the client socket.
Socket Socket_Accept(Socket);

#include "SocketPrivate.h"

#endif
