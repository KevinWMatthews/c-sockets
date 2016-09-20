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

    void setup()
    {
        mock().strictOrder();
        socket = Socket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }

    void expectSocketOpen(int socket_descriptor_or_error_code)
    {
        mock().expectOneCall("SocketSystemLayer_Open")
            .andReturnValue(socket_descriptor_or_error_code);
    }

    void expectSocketClose(int socket_descriptor, int return_code)
    {
        mock().expectOneCall("SocketSystemLayer_Close")
            .withParameter("descriptor", socket_descriptor)
            .andReturnValue(return_code);
    }
};

/*
 * Test List:
 *  Create:
 *
 *  Destroy:
 *      Null pointers.
 *      Double destroy.
 *      Do we need to check *self == 0?
 *
 *  Open:
 *
 *  Close:
 *
 *  Connect (client only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Bind (server only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Listen (server only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Accept (server only):
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Send:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Receive:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get handle [file descriptor]:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get IP address:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Get port:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Set option:
 *      Can fail.
 *      Will not crash with null pointer.
 */

TEST(Socket, it_can_create_and_destroy_a_socket)
{
}

TEST(Socket, it_can_destroy_a_socket_twice)
{
    Socket_Destroy(&socket);
}

TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetDescriptor(NULL) );
    Socket_Close(NULL);
}

TEST(Socket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(SOCKET_SYSTEM_LAYER_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor(socket) );
}

TEST(Socket, it_can_open_a_socket)
{
    int socket_descriptor = 42;

    expectSocketOpen(socket_descriptor);
    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );
    LONGS_EQUAL( socket_descriptor, Socket_GetDescriptor(socket) );
}

TEST(Socket, it_can_close_a_socket)
{
    int socket_descriptor = 43;

    expectSocketOpen(socket_descriptor);
    expectSocketClose(socket_descriptor, SOCKET_SYSTEM_LAYER_SUCCESS);
    Socket_Open(socket);
    Socket_Close(socket);
    LONGS_EQUAL( SOCKET_INVALID_DESCRIPTOR, Socket_GetDescriptor(socket) );
}
