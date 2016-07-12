#ifndef TEST_SOCKET_INCLUDED
#define TEST_SOCKET_INCLUDED

class Test_Socket
{
  public:
    explicit Test_Socket();
    virtual ~Test_Socket();

  private:
    Test_Socket(const Test_Socket&);
    Test_Socket& operator=(const Test_Socket&);
};

#endif
