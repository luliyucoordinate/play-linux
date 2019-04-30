#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

enum CMD{
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_ERROR
};

struct DataHeader {
    DataHeader(short dl=0, short cd=CMD_ERROR) : dataLength(dl), cmd(cd){}
    short dataLength; //data length
    short cmd;
};

struct Login : public DataHeader{
    Login() : DataHeader(sizeof(Login), CMD_LOGIN) {}

    char userName[32];
    char passWord[32];
};

struct LoginResult : public DataHeader {
    LoginResult() : DataHeader(sizeof(Login), CMD_LOGIN_RESULT), res(0) {}

    int res;
};

struct Logout : public DataHeader {
    Logout() : DataHeader(sizeof(Login), CMD_LOGOUT) {}

    char userName[32];
};

struct LogoutResult : public DataHeader {
    LogoutResult() : DataHeader(sizeof(Login), CMD_LOGOUT_RESULT), res(0) {}

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
            DataHeader header;

            //recv from client
            int nLen = recv(_cSock, (char *)&header, sizeof(header), 0);
            if (nLen <= 0) {
                printf("client quit");
                break;
            }
            
            switch (header.cmd)
            {
                case CMD_LOGIN:
                {
                    Login login = {};
                    recv(_cSock, (char*)&login+sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
                    printf("recv cmd_login ,data len:%d, userName=%s, passwd=%s\n", header.dataLength, login.userName, login.passWord);
                    LoginResult res;
                    send(_cSock, (char*)&res, sizeof(LoginResult), 0);
                }
                break;
                case CMD_LOGOUT:
                {
                    Logout logout = {};
                    recv(_cSock, (char*)&logout+ sizeof(DataHeader), sizeof(Login)- sizeof(DataHeader), 0);
                    printf("recv cmd_login ,data len:%d, userName=%s\n", header.dataLength, logout.userName);
                    LogoutResult res;
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