#ifndef UNIX_SOCKET_INCLUDED
#define UNIX_SOCKET_INCLUDED

typedef enum
{
    UNIX_SOCKET_FAIL = -1
} UnixSocketReturnCode;
int UnixSocket_Open(void);
void UnixSocket_Close(int);

#endif
