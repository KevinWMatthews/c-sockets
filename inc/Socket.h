#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

Socket Socket_Create(void);
void Socket_Destroy(Socket *);

typedef enum
{
    SOCKET_NULL_POINTER = -3,
    SOCKET_FAIL = -2,
    SOCKET_INVALID_DESCRIPTOR = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;
int Socket_Open(Socket);
void Socket_Close(Socket);
int Socket_GetDescriptor(Socket);

#endif
