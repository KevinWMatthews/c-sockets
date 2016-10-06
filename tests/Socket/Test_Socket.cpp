extern "C"
{
#include "Socket.h"
#include "SocketSystemLayer.h"
}

#include "Test_Socket.h"
#include "TestHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(Socket)
{
    Socket socket;
    int socket_descriptor;
    SocketSettingsStruct socket_settings_struct;
    SocketSettings socket_settings;
    SocketSettingsStruct udp_settings_struct;
    SocketSettings udp_settings;

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
        socket_descriptor = 42;
        socket_settings_struct.domain = SOCKET_DOMAIN_IPV4;
        socket_settings_struct.type = SOCKET_TYPE_STREAM;
        socket_settings_struct.protocol = SOCKET_PROTOCOL_DEFAULT;
        socket_settings = &socket_settings_struct;

        udp_settings_struct.domain = SOCKET_DOMAIN_IPV4;
        udp_settings_struct.type = SOCKET_TYPE_DATAGRAM;
        udp_settings_struct.protocol = SOCKET_PROTOCOL_DEFAULT;
        udp_settings = &udp_settings_struct;
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

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
 *  Set option:
 *      Can fail.
 *      Can set UDP Broadcast.
 *      Can set TCP Reuse Immediately.
 *
 *  Send:
 *      Add flags.
 *
 *  SendTo:
 *      Add flags.
 *
 *  Receive:
 *      Add flags.
 *
 *  GetDescriptor:
 *      Can fail.
 *
 *  GetIpAddress:
 *      Can fail.
 *
 *  GetPort:
 *      Can fail.
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

TEST(Socket, destroy_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
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

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, Socket_Open(socket, socket_settings) );

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

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, Socket_SetOptions(socket, &options) );
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

TEST(Socket, close_can_handle_null_pointers)
{
    Socket_Close(NULL);
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
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketReceive(socket_descriptor, receive_buffer, receive_buffer_length, number_of_bytes_read);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_read, Socket_Receive(socket, receive_buffer, receive_buffer_length) );
}

TEST(Socket, it_can_fail_to_receive)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[11] = {0};
    unsigned int receive_buffer_length = sizeof(receive_buffer) - 1;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketReceive(socket_descriptor, receive_buffer, receive_buffer_length, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, Socket_Receive(socket, receive_buffer, receive_buffer_length) );
}

TEST(Socket, it_will_not_receive_with_a_receive_null_buffer)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    unsigned int receive_buffer_length = 10;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Receive(socket, NULL, receive_buffer_length) );
}

TEST(Socket, it_will_not_receive_if_buffer_length_is_zero)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char receive_buffer[11] = {0};
    unsigned int receive_buffer_length = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

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
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketSend(socket_descriptor, message, message_length, number_of_bytes_sent);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( number_of_bytes_sent, Socket_Send(socket, message, message_length) );
}

TEST(Socket, it_can_fail_send_data_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketSend(socket_descriptor, message, message_length, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, Socket_Send(socket, message, message_length) );
}

TEST(Socket, it_will_not_send_a_null_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    unsigned int message_length = 7;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Send(socket, NULL, message_length) );
}

TEST(Socket, it_will_not_send_a_zero_length_message)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;
    char message[] = "Hello";
    unsigned int message_length = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);

    Socket_Open(socket, socket_settings);
    SocketClient_Connect(socket, ip_address, port);

    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_Send(socket, message, message_length) );
}

// SendTo
TEST(Socket, send_to_can_send_a_message)
{
    const char * ip_address = "10.10.0.1";
    int port = 12121;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketSendTo(socket_descriptor, message, message_length, ip_address, port, message_length);

    Socket_Open(socket, udp_settings);
    LONGS_EQUAL( message_length, Socket_SendTo(socket, message, message_length, ip_address, port) );
}

TEST(Socket, send_to_can_send_a_message_of_zero_length) // This is valid for UDP packets.
{
    const char * ip_address = "10.10.0.1";
    int port = 12121;
    char message[] = "";
    unsigned int message_length = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketSendTo(socket_descriptor, message, message_length, ip_address, port, message_length);

    Socket_Open(socket, udp_settings);
    LONGS_EQUAL( message_length, Socket_SendTo(socket, message, message_length, ip_address, port) );
}

TEST(Socket, send_to_can_detect_system_layer_failure)
{
    const char * ip_address = "10.10.0.1";
    int port = 12121;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketSendTo(socket_descriptor, message, message_length, ip_address, port, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, udp_settings);
    LONGS_EQUAL( SOCKET_SYSTEM_LAYER_FAIL, Socket_SendTo(socket, message, message_length, ip_address, port) );
}

TEST(Socket, send_to_will_not_transmit_a_null_message)
{
    const char * ip_address = "10.10.0.1";
    int port = 12121;
    unsigned int message_length = 7;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    Socket_Open(socket, udp_settings);
    LONGS_EQUAL( SOCKET_INVALID_BUFFER, Socket_SendTo(socket, NULL, message_length, ip_address, port) );
}

TEST(Socket, send_to_can_handle_a_null_socket)
{
    const char * ip_address = "10.10.0.1";
    int port = 12121;
    char message[] = "Hello";
    unsigned int message_length = sizeof(message);

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_DATAGRAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    Socket_Open(socket, udp_settings);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_SendTo(NULL, message, message_length, ip_address, port) );
}

// GetDescriptor
TEST(Socket, get_descriptor_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetDescriptor(NULL) );
}

// GetIpAddress
TEST(Socket, get_ip_address_can_handle_null_pointers)
{
    POINTERS_EQUAL( SOCKET_INVALID_IP_ADDRESS, Socket_GetIpAddress(NULL) );
}

// GetPort
TEST(Socket, get_port_can_handl_null_pointers)
{
    LONGS_EQUAL( SOCKET_INVALID_PORT, Socket_GetPort(NULL) );
}
