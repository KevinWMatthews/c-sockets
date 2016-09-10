extern "C"
{
#include "Socket.h"
}

#include "Test_Socket.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(Socket)
{
    Socket socket;

    void setup()
    {
        socket = Socket_Create();
    }

    void teardown()
    {
        Socket_Destroy(socket);
    }
};

TEST(Socket, it_can_create_and_destroy_a_socket)
{
}

TEST(Socket, it_can_destroy_a_socket_twice)
{
    Socket_Destroy(socket);
}

TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
}
