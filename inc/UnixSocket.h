#ifndef UNIX_SOCKET_INCLUDED
#define UNIX_SOCKET_INCLUDED

typedef enum
{
    UNIX_SOCKET_FAIL = -1,
    UNIX_SOCKET_SUCCESS = 0
} UnixSocketReturnCode;
int UnixSocket_Open(void);
void UnixSocket_Close(int);
int UnixSocket_Bind(int file_descriptor, const char * ip_address, int port);
int UnixSocket_Connect(int file_descriptor, const char * ip_address, int port);
int UnixSocket_Send(int file_descriptor, const char * message, unsigned int message_length);
int UnixSocket_Receive(int file_descriptor, char * buffer, unsigned int buffer_length);
int UnixSocket_Listen(int file_descriptor, int backlog);

#endif
