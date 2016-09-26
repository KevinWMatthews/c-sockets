#ifndef SOCKET_SYSTEM_LAYER_INCLUDED
#define SOCKET_SYSTEM_LAYER_INCLUDED

typedef enum
{
    SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE  = -2,
    SOCKET_SYSTEM_LAYER_FAIL            = -1,
    SOCKET_SYSTEM_LAYER_SUCCESS         =  0
} SocketSystemLayer_ReturnCode;
int SocketSystemLayer_Open(void);
int SocketSystemLayer_Close(int);
int SocketSystemLayer_Bind(int descriptor, const char * ip_address, int port);
int SocketSystemLayer_Listen(int descriptor, int backlog);
int SocketSystemLayer_Accept(int descriptor);
int SocketSystemLayer_Connect(int descriptor, const char * ip_address, int port);
int SocketSystemLayer_Receive(int descriptor, char * buffer, unsigned int buffer_length);

#endif
