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
    if (INVALID_SOCKET == _sock) {
        printf("socket error\n");
    }

    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(14567);
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
        printf("connect error\n");
    }
    
    
    while (true)
    {
        char cmdBuf[128] = {};
        scanf("%s", cmdBuf);
        if (0 == strcmp(cmdBuf, "exit")) {
            break;
        }
        else {
            send(_sock, cmdBuf, strlen(cmdBuf)+1, 0);
        }
        char recvbuf[128] = {};
        int nLen = recv(_sock, recvbuf, 128, 0);
        if (nLen > 0) {
            DataPackage* dp = (DataPackage*)recvbuf;
            printf("age:%d name:%s\n", dp->age ,dp->name);
        }
        else {
            printf("recv error\n");
        }
    }


    closesocket(_sock);
    WSACleanup();
    printf("client quit");
    getchar();
}