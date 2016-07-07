#ifndef UNIX_SOCKET_INCLUDED
#define UNIX_SOCKET_INCLUDED

typedef enum
{
    UNIX_SOCKET_FAIL = -1,
    UNIX_SOCKET_SUCCESS = 0
} UnixSocketReturnCode;
int UnixSocket_Open(void);
void UnixSocket_Close(int);
int UnixSocket_Connect(const char * ip_address, int port);

#endif
