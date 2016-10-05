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
};

/*
 * Test List:
 *  Connect (client only):
 *      Sanitize IP address and port.
 */
TEST(SocketClient, it_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(NULL, "192.168.2.1", 8888) );
}

// Connect
TEST(SocketClient, it_can_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Connect(socket, ip_address, port) );
}

TEST(SocketClient, connect_ip_address_can_not_be_null)
{
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(socket, NULL, port) );
}

TEST(SocketClient, it_can_fail_to_connect_to_a_socket)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketConnect(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, ip_address, port, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, Socket_Connect(socket, ip_address, port) );
}
