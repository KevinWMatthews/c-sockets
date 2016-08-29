extern "C"
{
#include "LinuxSocket.h"
}

#include "Test_LinuxSocket.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

/* Test List:
 *  Create()
 *      Create the interface
 *
 *  Destroy()
 *      Double destroy
 *      Figure out how to set the pointer to NULL.
 *
 *  Open()
 *  Close()
 *  Bind()
 *  Connect()
 *  Send()
 *  Receive()
 *  Listen()
 *  Accept()
 *  GetPort()
 *  GetIpAddress()
 *  GetFileDescriptor()
 */

TEST_GROUP(LinuxSocket)
{
    Socket socket;

    void setup()
    {
        mock().strictOrder();
        socket = LinuxSocket_Create();
    }

    void teardown()
    {
        LinuxSocket_Destroy(&socket);
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(LinuxSocket, it_can_create_and_destroy_a_socket)
{
    //TODO
    // POINTERS_EQUAL(NULL, socket);
}

TEST(LinuxSocket, it_can_handle_null_pointers)
{
    LinuxSocket_Destroy(&socket);
}
