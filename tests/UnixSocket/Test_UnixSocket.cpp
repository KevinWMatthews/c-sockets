extern "C"
{
#include "Socket.h"
#include "UnixSocket.h"
}

#include "Test_UnixSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

TEST_GROUP(UnixSocket)
{
    int file_descriptor;
    int file_descriptor2;
    Socket socket;
    Socket socket2;

    void setup()
    {
        mock().strictOrder();
        file_descriptor = 42;
        file_descriptor2 = 43;
        socket = Socket_Create();
        socket2 = Socket_Create();
    }

    void teardown()
    {
        Socket_Destroy(&socket);
        Socket_Destroy(&socket2);
        mock().checkExpectations();
        mock().clear();
    }

    void expectSocketOpen(int file_descriptor_or_error_code)
    {
        mock().expectOneCall("UnixSocket_Open")
            .andReturnValue(file_descriptor_or_error_code);
    }

    void expectSocketClose(int file_descriptor)
    {
        mock().expectOneCall("UnixSocket_Close")
            .withParameter("file_descriptor", file_descriptor);
    }

    void expectSocketConnect(const char * ip_address, int port, int result)
    {
        mock().expectOneCall("UnixSocket_Connect")
            .withParameter("ip_address", ip_address)
            .withParameter("port", port)
            .andReturnValue(result);
    }
};

/* Test List:
 *   Connect:
 *     Null pointer
 *     Fail
 *     Success
 */

TEST(UnixSocket, it_can_create_and_double_destroy_a_socket_struct)
{
    Socket_Destroy(&socket);
    POINTERS_EQUAL(NULL, socket);
    // Destroy happens again in teardown
}

TEST(UnixSocket, it_can_handle_null_pointers)
{
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_GetFileDescriptor(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Open(NULL) );
    LONGS_EQUAL( SOCKET_NULL_POINTER, Socket_Connect(NULL, "0.0.0.0", 0) );
}

TEST(UnixSocket, it_can_fail_to_open_a_socket)
{
    expectSocketOpen(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );
    LONGS_EQUAL( -1, Socket_GetFileDescriptor(socket) );
}

TEST(UnixSocket, it_can_open_a_socket)
{
    expectSocketOpen(file_descriptor);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );
    LONGS_EQUAL( 42, Socket_GetFileDescriptor(socket) );
}

TEST(UnixSocket, it_can_close_a_socket)
{
    expectSocketOpen(file_descriptor);
    expectSocketClose(file_descriptor);
    Socket_Open(socket);

    Socket_Close(socket);
}

TEST(UnixSocket, it_can_open_several_sockets)
{
    expectSocketOpen(file_descriptor);
    expectSocketOpen(file_descriptor2);

    Socket_Open(socket);
    Socket_Open(socket2);
}

TEST(UnixSocket, it_can_close_several_sockets)
{
    expectSocketOpen(file_descriptor);
    expectSocketOpen(file_descriptor2);
    expectSocketClose(file_descriptor);
    expectSocketClose(file_descriptor2);
    Socket_Open(socket);
    Socket_Open(socket2);

    Socket_Close(socket);
    Socket_Close(socket2);
}

TEST(UnixSocket, it_can_fail_to_connect_to_a_server)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(ip_address, port, UNIX_SOCKET_FAIL);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_FAIL, Socket_Connect(socket, ip_address, port) );

    Socket_Close(socket);
}

TEST(UnixSocket, it_can_connect_to_a_server)
{
    const char * ip_address = "192.168.2.1";
    int port = 10004;

    expectSocketOpen(file_descriptor);
    expectSocketConnect(ip_address, port, UNIX_SOCKET_SUCCESS);
    expectSocketClose(file_descriptor);

    Socket_Open(socket);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Connect(socket, ip_address, port) );

    Socket_Close(socket);
}
