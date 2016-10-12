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

/*
 * IPv4:    Internet socket based on IPv4.
 */
typedef enum
{
    SOCKET_SYSTEM_DOMAIN_IPV4 = AF_INET
} SocketSystemLayerDomain;
/*
 * Stream:      TCP/IP; connection-based.
 * Datagram:    UDP; connectionless.
 */
typedef enum
{
    SOCKET_SYSTEM_TYPE_STREAM = SOCK_STREAM,
    SOCKET_SYSTEM_TYPE_DATAGRAM = SOCK_DGRAM
} SocketSystemLayerType;
/*
 * Most socket types only support a single protocol so the default protocol may be used.
 * This is true for:
 *      stream sockets.
 *      datagram sockets.
 */
typedef enum
{
    SOCKET_SYSTEM_PROTOCOL_DEFAULT = 0,
    SOCKET_SYSTEM_PROTOCOL_TCP = IPPROTO_TCP,
    SOCKET_SYSTEM_PROTOCOL_UDP = IPPROTO_UDP
} SocketSystemLayerProtocol;
/*
 * Create a socket with the system.
 * Returns the file descriptor.
 * Returns < 0 on error.
 * Requires the domain, type, and protocol to be specified;
 * see above enums.
 */
int SocketSystemLayer_Open(int domain, int type, int protocol);

/*
 * Close the socket referenced by the given descriptor.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Close(int descriptor);

/*
 * Bind a socket to the given address and port.
 * Requires:
 *      The file descriptor returned by Open().
 *      The socket domain (IPv4, Unix, etc).
 *      The IP address.
 *      The port.
 * For server sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Bind(int descriptor, int domain, const char * ip_address, int port);

/*
 * Tell the system that the given socket will accept incoming connection requests using accept,
 * i.e. that the socket is passive.
 * The backlog is TODO.
 * For server sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Listen(int descriptor, int backlog);

/*
 * The given socket will accept the first connection in the queue/backlog.
 * If no connection requests are present, it will block while waiting for a connection.
 * For connection-based sockets only.
 * For server sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Accept(int descriptor);

/*
 * The socket with the given descriptor will attempt to start a connection
 * with a socket at the given IP address and port.
 * Does this block?
 * For connection-based sockets only.
 * For client sockets only.
 * On success, returns 0.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Connect(int descriptor, int domain, const char * ip_address, int port);

/*
 * Send the message to the socket with the given file descriptor.
 * The option MSG_NOSIGNAL is enabled by default. This suppresses SIGPIPE signal and
 * lets the user handle the error with a return code.
 * On success, returns the number of bytes that were sent.
 * On failure, returns < 0.
 */
int SocketSystemLayer_Send(int descriptor, const char * message, unsigned int message_length);

/* Send the message from the given socket to the given IP address and port.
 * For use with UDP sockets that have not had their default destination set using Connect().
 * On success, returns the number of bytes that were sent.
 * On failure, returns < 0.
 */
int SocketSystemLayer_SendTo(int descriptor, const char * message, unsigned int message_length, int domain, const char * ip_address, int port);

/*
 * Receive a message from the socket with the given descriptor.
 * The socket handles receive the message automatically.
 * It places the message in the given buffer.
 * If the buffer is overrun, then TODO.
 * If the socket is not open, TODO.
 * On success, returns the number of bytes that were received.
 * This can be 0 for a connection-less socket.
 * If the socket is connection-based, returns 0.
 * On failure, returns < 0.
 */
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
