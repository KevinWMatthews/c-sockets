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

    void setup()
    {
        mock().strictOrder();
        file_descriptor = 42;
        file_descriptor2 = 43;
    }

    void teardown()
    {
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

TEST(UnixSocket, it_can_fail_to_open_a_socket)
{
    expectOpenSocket(UNIX_SOCKET_FAIL);
    LONGS_EQUAL( SOCKET_FAIL, Socket_Open() );
}

TEST(UnixSocket, it_can_open_a_socket)
{
    expectOpenSocket(file_descriptor);

    LONGS_EQUAL( SOCKET_SUCCESS, Socket_Open() );
}

TEST(UnixSocket, it_can_close_a_socket)
{
    expectOpenSocket(file_descriptor);
    expectCloseSocket(file_descriptor);
    Socket_Open();

    Socket_Close();
}

TEST(UnixSocket, it_can_open_several_sockets)
{
    expectOpenSocket(file_descriptor);
    expectOpenSocket(file_descriptor2);

    Socket_Open();
    Socket_Open();
}

IGNORE_TEST(UnixSocket, it_can_close_several_sockets)
{
    expectOpenSocket(file_descriptor);
    expectOpenSocket(file_descriptor2);
    Socket_Open();
    Socket_Open();

    Socket_Close();
    Socket_Close();
}
