extern "C"
{
#include "UnixSocket.h"
}

#include "Test_UnixSocket.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP(UnixSocket)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(UnixSocket, it_can_fail)
{
    FAIL("Start here.");
}
