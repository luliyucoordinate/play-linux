#include "MyTcpServer.hpp"
#include <thread>

int main()
{
    MyTcpServer server;
    server.InitSocket();
    server.Bind(nullptr, 14567);
    server.Listen(5);
    
    while (server.IsRun())
    {
        server.OnRun();
    }
    server.Close();
    getchar();
    return 0;
}