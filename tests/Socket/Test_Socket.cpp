extern "C"
{
#include "Socket.h"
#include "SocketSystemLayer.h"
}

#include "Test_Socket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(Socket)
{
    Socket socket;
    int socket_descriptor;
    SocketSettingsStruct socket_settings_struct;
    SocketSettings socket_settings;

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
        socket_descriptor = 42;
        socket_settings_struct.domain = SOCKET_DOMAIN_IPV4;
        socket_settings_struct.type = SOCKET_TYPE_STREAM;
        socket_settings_struct.protocol = SOCKET_PROTOCOL_DEFAULT;
        socket_settings = &socket_settings_struct;
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }

    void expectSocketOpen(int socket_descriptor_or_error_code, int domain, int type, int protocol)
    {
        mock().expectOneCall("SocketSystemLayer_Open")
            .withParameter("domain", domain)
            .withParameter("type", type)
            .withParameter("protocol", protocol)
            .andReturnValue(socket_descriptor_or_error_code);
    }

    void expectSetOption(int socket_descriptor, int option_level, int option_name, int return_code)
    {
        int option_value = 1;
        mock().expectOneCall("SocketSystemLayer_SetOptions")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("option_level", option_level)
            .withParameter("option_name", option_name)
            .withParameter("option_value", option_value)
            .withParameter("option_length", sizeof(option_value))
            .andReturnValue(return_code);
    }

    void expectSocketClose(int socket_descriptor, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Close")
            .withParameter("descriptor", socket_descriptor)
            .andReturnValue(return_code);
    }

    void expectSocketBind(int socket_descriptor, int domain, const char *ip_address, int port, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Bind")
            .withParameter("domain", domain)
            .withParameter("descriptor", socket_descriptor)
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(return_code);
    }

    void expectSocketListen(int socket_descriptor, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Listen")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("backlog", 0)
            .andReturnValue(return_code);
    }

    void expectSocketAccept(int server_socket_descriptor, int client_socket_descriptor)
    {
        mock().expectOneCall("SocketSystemLayer_Accept")
            .withParameter("descriptor", server_socket_descriptor)
            .andReturnValue(client_socket_descriptor);
    }

    void expectSocketConnect(int socket_descriptor, const char *ip_address, int port, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Connect")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(return_code);
    }

    void expectSocketReceive(int socket_descriptor, char * buffer, unsigned int buffer_length, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Receive")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("buffer", buffer)
            .withParameter("buffer_length", buffer_length)
            .andReturnValue(return_code);
    }

    void expectSocketSend(int socket_descriptor, char * message, unsigned int message_length, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Send")
            .withParameter("descriptor", socket_descriptor)
            .withParameter("message", message)
            .withParameter("message_length", message_length)
            .andReturnValue(return_code);
    }
};

#define CHECK_SOCKET_RESET(socket) \
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor((socket)) ); \
    CHECK_SOCKET_ADDRESS_AND_PORT((socket), SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

#define CHECK_SOCKET_ADDRESS_AND_PORT(socket, address, port) \
    LONGS_EQUAL( (address), Socket_GetIpAddress((socket)) ); \
    LONGS_EQUAL( (port), Socket_GetPort((socket)) );

/*
 * Test List:
 *  Create:
 *
 *  Destroy:
 *      Do we need to check *self == 0?
 *
 *  Open:
 *      Do all open tests need to close the socket as well?
 *      UDP must work with default protocol.
 *
 *  Close:
 *      Return error code on failure?
 *
 *  Connect (client only):
 *      Sanitize IP address and port.
 *
 *  Bind (server only):
 *      Bind to any address.
 *
 *  Listen (server only):
 *
 *  Accept (server only):
 *
 *  Send:
 *      Add flags.
 *
 *  Receive:
 *      Add flags.
 *
 *  GetDescriptor]:
 *      Can fail.
 *
 *  GetIpAddress:
 *      Can fail.
 *
 *  GetPort:
 *      Can fail.
 *
 *  Set option:
 *      Can fail.
 *      Can set UDP Broadcast.
 *      Can set TCP Reuse Immediately.
 */

// Create and destroy
TEST(Socket, it_can_create_and_destroy_a_socket)
{
    CHECK_SOCKET_RESET(socket);
}

TEST(Socket, it_can_destroy_a_socket_twice)
{
    Socket_Destroy(&socket);
}

// Null pointers
TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetDescriptor(NULL) );
    Socket_Close(NULL);
    POINTERS_EQUAL( SOCKET_INVALID_IP_ADDRESS, Socket_GetIpAddress(NULL) );
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Listen(NULL) );
    LONGS_EQUAL( NULL, Socket_Accept(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(NULL, "192.168.2.1", 8888) );
}

TEST(Socket, bind_can_accept_null_pointers)
{
    const char * ip_address = "192.168.0.1";
    int port = 12345;

    Socket_Bind(NULL, ip_address, port);
}

// Open
TEST(Socket, it_can_open_a_socket)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket, socket_settings) );

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_open_a_tcp_socket)
{
    socket_settings_struct.domain = SOCKET_DOMAIN_IPV4;
    socket_settings_struct.type = SOCKET_TYPE_STREAM;
    socket_settings_struct.protocol = SOCKET_PROTOCOL_TCP;
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_TCP);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket, socket_settings) );

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_open_a_udp_socket)
{
    socket_settings_struct.domain = SOCKET_DOMAIN_IPV4;
    socket_settings_struct.type = SOCKET_TYPE_DATAGRAM;
    socket_settings_struct.protocol = SOCKET_PROTOCOL_UDP;
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_UDP);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket, socket_settings) );

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(SOCKET_SYSTEM_LAYER_FAIL, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket, socket_settings) );

    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
}

TEST(Socket, it_can_open_several_sockets)
{
    SocketSettingsStruct socket_settings_struct2 = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_STREAM,
        .protocol = SOCKET_PROTOCOL_DEFAULT
    };

    Socket socket2 = Socket_Create();
    int socket_descriptor2 = 66;
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketOpen(socket_descriptor2, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    Socket_Open(socket, socket_settings);
    Socket_Open(socket2, &socket_settings_struct2);

    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);
    LONGS_EQUAL( socket_descriptor2, Socket_GetDescriptor(socket2) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket2, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

    Socket_Destroy(&socket2);
}

TEST(Socket, it_can_not_open_the_same_socket_twice)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_ALREADY_OPEN, Socket_Open(socket, &socket_settings_struct) );
    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
}

TEST(Socket, open_will_fail_with_null_socket)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Open(NULL, socket_settings) );
}

TEST(Socket, open_will_fail_with_null_settings)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Open(socket, NULL) );
}

// SetOptions
TEST(Socket, it_can_set_broadcast_option_for_udp_sockets)
{
    socket_settings->type = SOCKET_TYPE_DATAGRAM;
    SocketOptionsStruct options = {
        .option_name = SOCKET_OPTION_UDP_BROADCAST
    };

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSetOption(socket_descriptor, SOCKET_SYSTEM_OPTION_LEVEL_SOCKET, SOCKET_SYSTEM_OPTION_BROADCAST, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_SetOptions(socket, &options) );
}

TEST(Socket, it_can_set_reuse_socket_address)
{
    SocketOptionsStruct options = {
        .option_name = SOCKET_OPTION_REUSE_ADDRESS
    };
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSetOption(socket_descriptor, SOCKET_SYSTEM_OPTION_LEVEL_SOCKET, SOCKET_SYSTEM_OPTION_REUSE_ADDRESS, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_SetOptions(socket, &options) );
}

TEST(Socket, set_options_fails_if_invalid_options)
{
    SocketOptionsStruct options;
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_INVALID_OPTION, Socket_SetOptions(socket, &options) );
}

TEST(Socket, set_options_fails_if_system_layer_fails)
{
    SocketOptionsStruct options = {
        .option_name = SOCKET_OPTION_UDP_BROADCAST
    };
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSetOption(socket_descriptor, SOCKET_SYSTEM_OPTION_LEVEL_SOCKET, SOCKET_SYSTEM_OPTION_BROADCAST, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_FAIL, Socket_SetOptions(socket, &options) );
}

TEST(Socket, set_options_fails_with_a_null_socket)
{
    SocketOptionsStruct options;
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_SetOptions(NULL, &options) );
}

TEST(Socket, set_options_fails_with_null_options)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_SetOptions(socket, NULL) );
}

// Close
TEST(Socket, it_can_close_a_socket)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    Socket_Close(socket);

    CHECK_SOCKET_RESET(socket);
}

TEST(Socket, it_can_close_several_sockets)
{
    Socket socket2 = Socket_Create();
    int socket_descriptor2 = 66;
    SocketSettingsStruct socket_settings_struct2 = {
        .domain = SOCKET_DOMAIN_IPV4,
        .type = SOCKET_TYPE_STREAM,
        .protocol = SOCKET_PROTOCOL_DEFAULT
    };

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketOpen(socket_descriptor2, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);
    expectSocketClose(socket_descriptor2, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    Socket_Open(socket2, &socket_settings_struct2);

    Socket_Close(socket);
    Socket_Close(socket2);

    CHECK_SOCKET_RESET(socket);
    CHECK_SOCKET_RESET(socket2);

    Socket_Destroy(&socket2);
}

TEST(Socket, it_will_not_close_a_socket_twice)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    // Note that there is only one call to the close.
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    Socket_Close(socket);

    Socket_Close(socket);

    CHECK_SOCKET_RESET(socket);
}

// Bind
TEST(Socket, it_can_bind_to_a_specific_ip_address_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Bind(socket, ip_address, port) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, ip_address, port);
}

TEST(Socket, closing_a_bound_socket_resets_ip_address_and_port)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT); 
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);

    Socket_Close(socket);
    CHECK_SOCKET_RESET(socket);
}

// TEST(Socket, it_can_bind_to_any_ip_address_and_port)
// {
//     // TODO
// }

TEST(Socket, it_can_not_bind_to_an_address_and_port_twice)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE);
    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_ADDRESS_IN_USE, Socket_Bind(socket, ip_address, port) );
    CHECK_SOCKET_ADDRESS_AND_PORT(socket, ip_address, port);
}

TEST(Socket, it_can_fail_to_bind)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_FAIL);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Bind(socket, ip_address, port) );

    CHECK_SOCKET_ADDRESS_AND_PORT(socket, SOCKET_INVALID_IP_ADDRESS, SOCKET_INVALID_PORT);

    Socket_Close(socket);
}

// Listen
TEST(Socket, it_can_listen)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Listen(socket) );
}

TEST(Socket, it_can_fail_to_listen)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Listen(socket) );
}

// Accept
TEST(Socket, a_server_can_accpet_a_connection)
{
    Socket new_socket = {0};
    int new_socket_descriptor = 66;
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketAccept(socket_descriptor, new_socket_descriptor);

    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);
    Socket_Listen(socket);

    new_socket = Socket_Accept(socket);
    LONGS_EQUAL( new_socket_descriptor, Socket_GetDescriptor(new_socket) );
    Socket_Destroy(&new_socket);
}

TEST(Socket, it_can_fail_to_accept)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketAccept(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    Socket_Bind(socket, ip_address, port);
    Socket_Listen(socket);

    LONGS_EQUAL( NULL, Socket_Accept(socket) );
}

// Connect
TEST(Socket, it_can_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Connect(socket, ip_address, port) );
}

TEST(Socket, connect_ip_address_can_not_be_null)
{
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(socket, NULL, port) );
}

TEST(Socket, it_can_fail_to_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Connect(socket, ip_address, port) );
}

// Receive
TEST(Socket, it_can_receive_from_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[11] = {0};
    unsigned int receive_buffer_length = sizeof(receive_buffer) - 1;
    int number_of_bytes_read = 1;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketReceive(socket_descriptor, receive_buffer, receive_buffer_length, number_of_bytes_read);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_read, Socket_Receive(socket, receive_buffer, receive_buffer_length) );
}

TEST(Socket, it_can_fail_to_receive)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[11] = {0};
    unsigned int receive_buffer_length = sizeof(receive_buffer) - 1;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketReceive(socket_descriptor, receive_buffer, receive_buffer_length, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Receive(socket, receive_buffer, receive_buffer_length) );
}

TEST(Socket, it_will_not_receive_with_a_receive_null_buffer)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    unsigned int receive_buffer_length = 10;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Receive(socket, NULL, receive_buffer_length) );
}

TEST(Socket, it_will_not_receive_if_buffer_length_is_zero)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[11] = {0};
    unsigned int receive_buffer_length = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Receive(socket, receive_buffer, receive_buffer_length) );
}

TEST(Socket, can_not_receive_from_a_null_socket)
{
    char message_buffer[2] = {0};
    unsigned int message_length = sizeof(message_buffer) - 1;

    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Receive(NULL, message_buffer, message_length) );
}

// Send
TEST(Socket, it_can_send_data_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "A";
    unsigned int message_length = sizeof(message);
    int number_of_bytes_sent = 1;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketSend(socket_descriptor, message, message_length, number_of_bytes_sent);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_sent, Socket_Send(socket, message, message_length) );
}

TEST(Socket, it_can_fail_send_data_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketSend(socket_descriptor, message, message_length, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Send(socket, message, message_length) );
}

TEST(Socket, it_will_not_send_a_null_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    unsigned int message_length = 7;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Send(socket, NULL, message_length) );
}

TEST(Socket, it_will_not_send_a_zero_length_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    Socket_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Send(socket, message, message_length) );
}
