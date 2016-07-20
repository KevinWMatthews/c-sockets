#ifndef TEST_LINUX_SOCKET_INCLUDED
#define TEST_LINUX_SOCKET_INCLUDED

class Test_LinuxSocket
{
  public:
    explicit Test_LinuxSocket();
    virtual ~Test_LinuxSocket();

  private:
    Test_LinuxSocket(const Test_LinuxSocket&);
    Test_LinuxSocket& operator=(const Test_LinuxSocket&);
};

#endif
