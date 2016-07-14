#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef struct SocketStruct * Socket;

Socket Socket_Create(void);
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

int Socket_GetFileDescriptor(Socket);
const char * Socket_GetIpAddress(Socket);
int Socket_GetPort(Socket);

int Socket_Bind(Socket, const char * ip_address, int port);
int Socket_Connect(Socket, const char * ip_address, int port);
int Socket_Send(Socket, const char * message, unsigned int message_length);
int Socket_Receive(Socket, char * buffer, unsigned int buffer_length);
int Socket_Listen(Socket, int backlog);
Socket Socket_Accept(Socket);

#endif
