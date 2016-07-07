#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

Socket Socket_Create(void);
void Socket_Destroy(Socket *);

typedef enum
{
    SOCKET_NULL_POINTER = -2,
    SOCKET_FAIL = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;
int Socket_Open(Socket);

void Socket_Close(Socket);

int Socket_GetFileDescriptor(Socket);
int Socket_Connect(Socket, const char * ip_address, int port);
int Socket_Send(Socket, const char * message, unsigned int message_length);

#endif
