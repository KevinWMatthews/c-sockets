extern "C"
{
#include "Socket.h"
}

#include "Test_Socket.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(Socket)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Socket, it_can_fail)
{
    FAIL("Start here.");
}
