#ifndef SOCKET_SYSTEM_LAYER_INCLUDED
#define SOCKET_SYSTEM_LAYER_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef enum
{
    SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE  = -2,
    SOCKET_SYSTEM_LAYER_FAIL            = -1,
    SOCKET_SYSTEM_LAYER_SUCCESS         =  0
} SocketSystemLayer_ReturnCode;

typedef enum
{
    SOCKET_SYSTEM_DOMAIN_IPV4 = AF_INET
} SocketSystemLayerDomain;
typedef enum
{
    SOCKET_SYSTEM_TYPE_STREAM = SOCK_STREAM,
    SOCKET_SYSTEM_TYPE_DATAGRAM = SOCK_DGRAM
} SocketSystemLayerType;
typedef enum
{
    SOCKET_SYSTEM_PROTOCOL_DEFAULT = 0,
    SOCKET_SYSTEM_PROTOCOL_TCP = IPPROTO_TCP,
    SOCKET_SYSTEM_PROTOCOL_UDP = IPPROTO_UDP
} SocketSystemLayerProtocol;
int SocketSystemLayer_Open(int domain, int type, int protocol);
int SocketSystemLayer_Close(int);
int SocketSystemLayer_Bind(int descriptor, int domain, const char * ip_address, int port);
int SocketSystemLayer_Listen(int descriptor, int backlog);
int SocketSystemLayer_Accept(int descriptor);
int SocketSystemLayer_Connect(int descriptor, int domain, const char * ip_address, int port);
int SocketSystemLayer_Send(int descriptor, const char * message, unsigned int message_length);
int SocketSystemLayer_Receive(int descriptor, char * buffer, unsigned int buffer_length);

typedef enum
{
    SOCKET_SYSTEM_OPTION_LEVEL_SOCKET = SOL_SOCKET
} SocketSystemLayer_OptionLevel;

typedef enum
{
    SOCKET_SYSTEM_OPTION_BROADCAST = SO_BROADCAST,
    SOCKET_SYSTEM_OPTION_REUSE_ADDRESS = SO_REUSEADDR
} SocketSystemLayer_OptionName;
int SocketSystemLayer_SetOptions(int descriptor, int option_level, int option_name,
        void * option_value, int option_length);

#endif
