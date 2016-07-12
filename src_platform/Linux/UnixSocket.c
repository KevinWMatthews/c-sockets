#include "UnixSocket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int UnixSocket_Open(void)
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

void UnixSocket_Close(int file_descriptor)
{
    close(file_descriptor);
}

int UnixSocket_Bind(int file_descriptor, const char * ip_address, int port)
{
    struct sockaddr_in socket;
    socket.sin_family = AF_INET;
    socket.sin_addr.s_addr = INADDR_ANY;
    socket.sin_port = htons(port);
    return bind( file_descriptor, (struct sockaddr *)&socket, sizeof(socket) );
}

int UnixSocket_Connect(int file_descriptor, const char * ip_address, int port)
{
    struct sockaddr_in socket;
    socket.sin_family = AF_INET;
    socket.sin_addr.s_addr = inet_addr(ip_address);
    socket.sin_port = htons(port);
    return connect( file_descriptor, (struct sockaddr *)&socket, sizeof(socket) );
}

int UnixSocket_Send(int file_descriptor, const char * message, unsigned int message_length)
{
    return send(file_descriptor, message, message_length, 0);
}

int UnixSocket_Receive(int file_descriptor, char * buffer, unsigned int buffer_length)
{
    return recv(file_descriptor, buffer, buffer_length, 0);
}

int UnixSocket_Listen(int file_descriptor, int backlog)
{
    return 0;
}
