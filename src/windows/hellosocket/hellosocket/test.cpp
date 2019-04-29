#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

//#pragma comment(lib, "ws2_32.lib")
int main() 
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;

    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    if (bind(_sock, (sockaddr*)&_sin, sizeof _sin) == SOCKET_ERROR) {
        printf("error");
    }
    if (SOCKET_ERROR == listen(_sock, 5)) printf("error");
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    while (true)
    {
        SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
        char buf[] = "hell0";
        send(_cSock, buf, strlen(buf) + 1, 0);
    }
    
    closesocket(_sock);
    WSAStartup(ver, &dat);
    WSACleanup();
}