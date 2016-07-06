#ifndef TEST_UNIX_SOCKET_INCLUDED
#define TEST_UNIX_SOCKET_INCLUDED

class Test_UnixSocket
{
  public:
    explicit Test_UnixSocket();
    virtual ~Test_UnixSocket();

  private:
    Test_UnixSocket(const Test_UnixSocket&);
    Test_UnixSocket& operator=(const Test_UnixSocket&);
};

#endif
