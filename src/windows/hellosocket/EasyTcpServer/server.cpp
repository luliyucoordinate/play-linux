#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")


struct DataPackage {
    int age;
    char name[32];
};

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);

    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(14567);
    _sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(_sock, (sockaddr*)&_sin, sizeof _sin) == SOCKET_ERROR) {
        printf("bind error\n");
    }
    if (SOCKET_ERROR == listen(_sock, 5)) printf("listen error \n");

    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    

    while (true)
    {
        SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
        if (INVALID_SOCKET == _cSock)
        {
            printf("error, none clinet!\n");
        }
        printf("new clinet:IP=%s\n", inet_ntoa(clientAddr.sin_addr));

        char _recvBuf[128] = {};
        while (true)
        {
            //recv from client
            int nLen = recv(_cSock, _recvBuf, 128, 0);
            if (nLen <= 0) {
                printf("client quit");
                break;
            }
            printf("recv cmd : %s\n", _recvBuf);
            if (0 == strcmp(_recvBuf, "getInfo")) {
                DataPackage dp = { 80, "xiao xiao" };
                send(_cSock, (const char *)&dp, sizeof(DataPackage), 0);
            }
            else {
                char buf[] = "???";
                send(_cSock, buf, strlen(buf) + 1, 0);
            }
        }
    }
   
    closesocket(_sock);
    WSACleanup();
    printf("server quit");
    getchar();
}