#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include <vector>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

enum CMD{
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_NEW_USER_JOIN,
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

struct NewUserJoin : public DataHeader {
    NewUserJoin() : DataHeader(sizeof(Login), CMD_NEW_USER_JOIN), sock(0) {}

    int sock;
};

vector<SOCKET> g_clients;

int processor(SOCKET _cSock)
{
    DataHeader header;

    //recv from client
    int nLen = recv(_cSock, (char *)&header, sizeof(header), 0);
    if (nLen <= 0) {
        printf("client<Socket=%d> quit\n", _cSock);
        return -1;
    }

    switch (header.cmd)
    {
    case CMD_LOGIN:
    {
        Login login = {};
        recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
        printf("recv cmd_login ,data len:%d, userName=%s, passwd=%s\n", header.dataLength, login.userName, login.passWord);
        LoginResult res;
        send(_cSock, (char*)&res, sizeof(LoginResult), 0);
    }
    break;
    case CMD_LOGOUT:
    {
        Logout logout = {};
        recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
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

    while (true)
    {
        fd_set fdRead, fdWrite, fdExcept;

        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExcept);

        FD_SET(_sock, &fdRead);
        FD_SET(_sock, &fdWrite);
        FD_SET(_sock, &fdExcept);

        for (size_t i = 0; i < g_clients.size(); ++i)
        {
            FD_SET(g_clients[i], &fdRead);
        }

        timeval t = { 0, 0 };
        int res = select(_sock + 1, &fdRead, &fdWrite, &fdExcept, &t);
        if (res < 0)
        {
            printf("select error\n");
            break;
        }

        if (FD_ISSET(_sock, &fdRead))
        {
            FD_CLR(_sock, &fdRead);
            sockaddr_in clientAddr = {};
            int nAddrLen = sizeof(sockaddr_in);
            SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
            if (INVALID_SOCKET == _cSock)
            {
                printf("error, none client!\n");
            }
            for (size_t i = 0; i < g_clients.size(); ++i)
            {
                NewUserJoin userJoin;
                send(g_clients[i], (const char*)&userJoin, sizeof(NewUserJoin), 0);
            }
            g_clients.push_back(_cSock);
            printf("new client:IP=%s\n", inet_ntoa(clientAddr.sin_addr));
        }


        for (size_t i = 0; i < fdRead.fd_count; ++i)
        {
            if (-1 == processor(fdRead.fd_array[i]))
            {
                auto iter = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[i]);
                if (iter != g_clients.end()) g_clients.erase(iter);
            }
        }
    }
   
    for (size_t i = 0; i < g_clients.size(); ++i)
    {
        closesocket(g_clients[i]);
    }
    closesocket(_sock);
    WSACleanup();
    printf("server quit");
    getchar();
}