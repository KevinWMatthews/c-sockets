#include "UnixSocket.h"

int UnixSocket_Open(void)
{
    return 0;
}

void UnixSocket_Close(int file_descriptor)
{
}

int UnixSocket_Connect(int file_descriptor, const char * ip_address, int port)
{
    return 0;
}

int UnixSocket_Send(int file_descriptor, const char * message, unsigned int message_length)
{
    return 0;
}

int UnixSocket_Receive(int file_descriptor, char * buffer, unsigned int buffer_length)
{
    return 0;
}
