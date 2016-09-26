#include "SocketSystemLayer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef enum
{
    SOCKET_SYSTEM_DOMAIN_IPV4 = AF_INET
} SocketSystemLayer_Domain;
typedef enum
{
    SOCKET_SYSTEM_TYPE_STREAM = SOCK_STREAM
} SocketSystemLayer_Type;
typedef enum
{
    SOCKET_SYSTEM_PROTOCOL_DEFAULT = 0
} SocketSystemLayer_Protocol;

int SocketSystemLayer_Open(void)
{
    return socket(SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
}

int SocketSystemLayer_Close(int descriptor)
{
    return close(descriptor);
}

int SocketSystemLayer_Bind(int descriptor, const char * ip_address, int port)
{
    struct sockaddr_in socket;
    socket.sin_family = AF_INET;    //TODO this is hard-coded and should be an argument!
    socket.sin_addr.s_addr = inet_addr(ip_address);
    socket.sin_port = htons(port);
    // We defined an internet socket (sockaddr_in) instead of a standard socket(sockaddr).
    // Conveniently, their pointers can be typecast.
    return bind( descriptor, (struct sockaddr *)&socket, sizeof(socket) );
}

int SocketSystemLayer_Listen(int descriptor, int backlog)
{
    return listen(descriptor, backlog);
}

int SocketSystemLayer_Accept(int descriptor)
{
    // The other two arguments can be used to populate sockaddr and socklen_t
    // structs with the details of the client connection.
    // There is no need to do this yet.
    return accept(descriptor, 0, 0);
}

int SocketSystemLayer_Connect(int descriptor, const char * ip_address, int port)
{
    struct sockaddr_in socket;
    socket.sin_family = AF_INET;    //TODO this is hard-coded and should be an argument!
    //TODO extract packaging the address and port. Bind() does this too.
    socket.sin_addr.s_addr = inet_addr(ip_address);
    socket.sin_port = htons(port);
    return connect( descriptor, (struct sockaddr *)&socket, sizeof(socket) );
}

int SocketSystemLayer_Send(int descriptor, const char * message, unsigned int message_length)
{
    // MSG_NOSIGNAL suppresses SIGPIPE signal and lets the user handle the error.
    return send(descriptor, message, message_length, MSG_NOSIGNAL);
}

int SocketSystemLayer_Receive(int descriptor, char * buffer, unsigned int buffer_length)
{
    return recv(descriptor, buffer, buffer_length, 0);
}
