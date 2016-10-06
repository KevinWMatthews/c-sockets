extern "C"
{
#include "Socket.h"
#include "SocketSystemLayer.h"
}

#include "Test_Socket.h"
#include "TestHelpers.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(SocketServer)
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

        socket_address_struct.ip_address = "192.168.2.1";
        socket_address_struct.port = 10004;
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
 *  Bind (server only):
 *      Bind to any address.
 *
 *  Listen (server only):
 *
 *  Accept (server only):
 *
 */

// Bind
TEST(SocketServer, it_can_bind_to_a_specific_ip_address_and_port)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);

    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_SUCCESS, SocketServer_Bind(socket, socket_address) );
}

TEST(SocketServer, closing_a_bound_socket_resets_ip_address_and_port)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);

    Socket_Close(socket);
    CHECK_SOCKET_CLOSED(socket);
}

// TEST(SocketServer, it_can_bind_to_any_ip_address_and_port)
// {
//     // TODO
// }

TEST(SocketServer, it_can_not_bind_to_an_address_and_port_twice)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_ADDRESS_IN_USE);
    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);

    LONGS_EQUAL( SOCKET_ADDRESS_IN_USE, SocketServer_Bind(socket, socket_address) );
}

TEST(SocketServer, it_can_fail_to_bind)
{
    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_FAIL);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, SocketServer_Bind(socket, socket_address) );

    Socket_Close(socket);
}

TEST(SocketServer, bind_can_accept_null_socket)
{
    SocketServer_Bind(NULL, socket_address);
}

IGNORE_TEST(SocketServer, bind_can_accept_null_socket_address)
{
}

// Listen
TEST(SocketServer, it_can_listen)
{
    int backlog = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, backlog, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);

    LONGS_EQUAL( SOCKET_SUCCESS, SocketServer_Listen(socket, backlog) );
}

TEST(SocketServer, it_can_listen_with_a_custom_backlog)
{
    int backlog = 1;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, backlog, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);

    LONGS_EQUAL( SOCKET_SUCCESS, SocketServer_Listen(socket, backlog) );
}

TEST(SocketServer, it_can_fail_to_listen)
{
    int backlog = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, backlog, SOCKET_SYSTEM_LAYER_FAIL);
    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);

    LONGS_EQUAL( SOCKET_FAILED_SYSTEM_CALL, SocketServer_Listen(socket, backlog) );
}

TEST(SocketServer, listen_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, SocketServer_Listen(NULL, 0) );
}

// Accept
TEST(SocketServer, a_server_can_accpet_a_connection)
{
    Socket new_socket = {0};
    int new_socket_descriptor = 66;
    int backlog = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, backlog, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketAccept(socket_descriptor, new_socket_descriptor);

    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);
    SocketServer_Listen(socket, backlog);

    new_socket = SocketServer_Accept(socket);
    LONGS_EQUAL( new_socket_descriptor, Socket_GetDescriptor(new_socket) );
    Socket_Destroy(&new_socket);
}

TEST(SocketServer, it_can_fail_to_accept)
{
    int backlog = 0;

    expectSocketOpen(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, SOCKET_SYSTEM_TYPE_STREAM, SOCKET_SYSTEM_PROTOCOL_DEFAULT);
    expectSocketBind(socket_descriptor, SOCKET_SYSTEM_DOMAIN_IPV4, socket_address, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketListen(socket_descriptor, backlog, SOCKET_SYSTEM_LAYER_SUCCESS);
    expectSocketAccept(socket_descriptor, SOCKET_SYSTEM_LAYER_FAIL);

    Socket_Open(socket, socket_settings);
    SocketServer_Bind(socket, socket_address);
    SocketServer_Listen(socket, backlog);

    LONGS_EQUAL( NULL, SocketServer_Accept(socket) );
}

TEST(SocketServer, accept_can_handle_null_pointers)
{
    LONGS_EQUAL( NULL, SocketServer_Accept(NULL) );
}
