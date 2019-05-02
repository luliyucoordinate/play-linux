#include "MyTcpClient.hpp"
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
    const int cnt = 1000;
    MyTcpClient *client[cnt];
    for (int i = 0; i < cnt; ++i)
    {
        if (!g_bRun) return 0;
        client[i] = new MyTcpClient();
    }

    for (int i = 0; i < cnt; ++i)
    {
        if (!g_bRun) return 0;
        client[i]->InitSocket();
        client[i]->Connect("127.0.0.1", 14567);
    }
    

    std::thread t1(cmdThread);
    t1.detach();

    Login login;
    strcpy(login.userName, "lly");
    strcpy(login.passWord, "lly");
    while (g_bRun)
    {
        for (int i = 0; i < cnt; ++i)
        {
            client[i]->SendData(&login);
            //client[i]->OnRun();
        }
    }
   
    for (int i = 0; i < cnt; ++i) {
        client[i]->Close();
    }

    return 0;
}