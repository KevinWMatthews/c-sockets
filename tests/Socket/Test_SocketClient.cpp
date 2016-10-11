extern "C"
{
#include "Socket.h"
#include "SocketSystemLayer.h"
}

#include "Test_Socket.h"
#include "TestHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(SocketClient)
{
    Socket socket;
    int socket_descriptor;
    SocketSettingsStruct socket_settings_struct;
    SocketSettings socket_settings;
    SocketAddressStruct socket_address_struct;
    SocketAddress socket_address;

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
        socket_descriptor = 42;
        socket_settings_struct.domain = SOCKET_DOMAIN_IPV4;
        socket_settings_struct.type = SOCKET_TYPE_STREAM;
        socket_settings_struct.protocol = SOCKET_PROTOCOL_DEFAULT;
        socket_settings = &socket_settings_struct;

        socket_address_struct.ip_address = "10.10.0.1";
        socket_address_struct.port = 10010;
        socket_address = &socket_address_struct;
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
 *  Connect (client only):
 *      Sanitize IP address and port.
 */
TEST(SocketClient, it_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, SocketClient_Connect(NULL, socket_address) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, SocketClient_Connect(socket, NULL) );
}

// Connect
TEST(SocketClient, it_can_connect_to_a_socket)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, SocketClient_Connect(socket, socket_address) );
}

TEST(SocketClient, connect_ip_address_can_not_be_null)
{
    socket_address->ip_address = NULL;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_NULL_POINTER, SocketClient_Connect(socket, socket_address) );
}

TEST(SocketClient, it_can_fail_to_connect_to_a_socket)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, SocketClient_Connect(socket, socket_address) );
}
