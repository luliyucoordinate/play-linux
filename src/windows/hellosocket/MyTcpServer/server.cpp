#include "MyTcpServer.hpp"
#include <thread>

bool g_bRun = true;
void cmdThread()
{
    while (true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            g_bRun = false;
            printf("client exit\n");
            return;
        }
        else
        {
            printf("do not apply\n");
        }
    }
}

int main()
{
    MyTcpServer server;
    server.InitSocket();
    server.Bind(nullptr, 14567);
    server.Listen(5);
    
    std::thread t1(cmdThread);
    t1.detach();

    while (g_bRun) {
        server.OnRun();
    }
    //while (server.IsRun())
    //{
    //    server.OnRun();
    //}
    server.Close();
    getchar();
    return 0;
}