#ifndef SOCKET_SERVER_INCLUDED
#define SOCKET_SERVER_INCLUDED

#include "Socket.h"

int Socket_Bind(Socket, const char * ip_address, int port);
int Socket_Listen(Socket, int backlog);
// Returns a handle to the client socket.
Socket Socket_Accept(Socket);

#endif
