#include "MyTcpClient.hpp"
#include <thread>

void cmdThread(MyTcpClient* client)
{
    while (true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            client->Close();
            printf("client exit\n");
            return;
        }
        else if (0 == strcmp(cmdBuf, "login"))
        {
            Login login;
            strcpy(login.userName, "lly");
            strcpy(login.passWord, "lly");
            client->SendData(&login);
        }
        else if (0 == strcmp(cmdBuf, "logout"))
        {
            Logout logout;
            client->SendData(&logout);
        }
        else
        {
            printf("do not apply\n");
        }
    }
}

int main()
{
    MyTcpClient client;
    client.InitSocket();
    client.Connect("127.0.0.1", 14567);

    std::thread t1(cmdThread, &client);
    t1.detach();

    while (client.IsRun())
    {
        client.OnRun();
    }
    client.Close();
    getchar();
    return 0;
}