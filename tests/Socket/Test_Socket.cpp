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

/*
 * Test List:
 *  Create:
 *
 *  Destroy:
 *      Null pointers.
 *      Double destroy.
 *
 *  Open:
 *      Can fail.
 *      Will not crash with null pointer.
 *
 *  Close:
 *      Can fail.
 *      Will not crash with null pointer.
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
    Socket_Destroy(socket);
}

TEST(Socket, it_can_handle_null_pointers)
{
    Socket_Destroy(NULL);
}
