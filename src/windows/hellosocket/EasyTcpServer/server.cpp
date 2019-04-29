#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

enum CMD{
    CMD_LOGIN,
    CMD_LOGOUT,
    CMD_ERROR
};

struct DataHeader {
    short dataLength; //data length
    short cmd;
};

struct Login {
    char userName[32];
    char passWord[32];
};

struct LoginResult {
    int res;
};

struct Logout {
    char username[32];
};

struct LogoutResult {
    int res;
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

        
        while (true)
        {
            DataHeader header = {};

            //recv from client
            int nLen = recv(_cSock, (char *)&header, sizeof(header), 0);
            if (nLen <= 0) {
                printf("client quit");
                break;
            }
            printf("recv cmd : %d, data len:%d\n", header.cmd, header.dataLength);
            switch (header.cmd)
            {
                case CMD_LOGIN:
                {
                    Login login = {};
                    recv(_cSock, (char*)&login, sizeof(Login), 0);
                    LoginResult res = {};
                    send(_cSock, (char*)&header, sizeof(DataHeader), 0);
                    send(_cSock, (char*)&res, sizeof(LoginResult), 0);
                }
                break;
                case CMD_LOGOUT:
                {
                    Logout Logout = {};
                    recv(_cSock, (char*)&Logout, sizeof(Login), 0);
                    LogoutResult res = {1};
                    send(_cSock, (char*)&header, sizeof(DataHeader), 0);
                    send(_cSock, (char*)&res, sizeof(LogoutResult), 0);
                }
                break;
                default:
                    header.cmd = CMD_ERROR;
                    header.dataLength = 0;
                    send(_cSock, (char*)&header, sizeof(header), 0);
                    break;
            }
        }
    }
   
    closesocket(_sock);
    WSACleanup();
    printf("server quit");
    getchar();
}