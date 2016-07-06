#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

typedef enum
{
    SOCKET_FAIL = -1,
    SOCKET_SUCCESS = 0
} SocketReturnCode;
int Socket_Open(void);
void Socket_Close(void);

#endif
