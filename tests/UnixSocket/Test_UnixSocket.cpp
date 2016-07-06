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

    void expectOpenSocket(int file_descriptor_or_error_code)
    {
        mock().expectOneCall("UnixSocket_Open")
            .andReturnValue(file_descriptor_or_error_code);
    }

    void expectCloseSocket(int file_descriptor)
    {
        mock().expectOneCall("UnixSocket_Close")
            .withParameter("file_descriptor", file_descriptor);
    }
};

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
}

TEST(UnixSocket, it_can_fail_to_open_a_socket)
{
    expectOpenSocket(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open(socket) );
    LONGS_EQUAL( -1, Socket_GetFileDescriptor(socket) );
}

TEST(UnixSocket, it_can_open_a_socket)
{
    expectOpenSocket(file_descriptor);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open(socket) );
    LONGS_EQUAL( 42, Socket_GetFileDescriptor(socket) );
}

TEST(UnixSocket, it_can_close_a_socket)
{
    expectOpenSocket(file_descriptor);
    expectCloseSocket(file_descriptor);
    Socket_Open(socket);

    Socket_Close(socket);
}

TEST(UnixSocket, it_can_open_several_sockets)
{
    expectOpenSocket(file_descriptor);
    expectOpenSocket(file_descriptor2);

    Socket_Open(socket);
    Socket_Open(socket2);
}

TEST(UnixSocket, it_can_close_several_sockets)
{
    expectOpenSocket(file_descriptor);
    expectOpenSocket(file_descriptor2);
    expectCloseSocket(file_descriptor);
    expectCloseSocket(file_descriptor2);
    Socket_Open(socket);
    Socket_Open(socket2);

    Socket_Close(socket);
    Socket_Close(socket2);
}
