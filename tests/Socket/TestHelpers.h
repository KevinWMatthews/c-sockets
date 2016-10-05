#ifndef TEST_HELPERS_INCLUDED
#define TEST_HELPERS_INCLUDED

#define CHECK_SOCKET_RESET(socket) \
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor((socket)) ); \
    CHECK_SOCKET_ADDRESS_AND_PORT((socket), SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

#define CHECK_SOCKET_ADDRESS_AND_PORT(socket, address, port) \
    LONGS_EQUAL( (address), Socket_GetIpAddress((socket)) ); \
    LONGS_EQUAL( (port), Socket_GetPort((socket)) );

// All sockets
void expectSocketOpen(int socket_descriptor_or_error_code, int domain, int type, int protocol);
void expectSocketClose(int socket_descriptor, int return_code);
void expectSetOption(int socket_descriptor, int option_level, int option_name, int return_code);
void expectSocketReceive(int socket_descriptor, char * buffer, unsigned int buffer_length, int return_code);
void expectSocketSend(int socket_descriptor, char * message, unsigned int message_length, int return_code);

// Client sockets
void expectSocketConnect(int socket_descriptor, int domain, const char *ip_address, int port, int return_code);

// Server sockets
void expectSocketBind(int socket_descriptor, int domain, const char *ip_address, int port, int return_code);
void expectSocketListen(int socket_descriptor, int backlog, int return_code);
void expectSocketAccept(int server_socket_descriptor, int client_socket_descriptor);

#endif
