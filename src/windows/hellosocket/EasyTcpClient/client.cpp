#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

enum CMD {
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_NEW_USER_JOIN,
    CMD_ERROR
};

struct DataHeader {
    DataHeader(short dl = 0, short cd = CMD_ERROR) : dataLength(dl), cmd(cd) {}
    short dataLength; //data length
    short cmd;
};

struct Login : public DataHeader {
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

int processor(SOCKET _cSock)
{
    DataHeader header;

    //recv from client
    int nLen = recv(_cSock, (char *)&header, sizeof(header), 0);
    if (nLen <= 0) {
        printf("bye to server\n", _cSock);
        return -1;
    }

    switch (header.cmd)
    {
    case CMD_LOGIN_RESULT:
    {
        LoginResult loginRes = {};
        recv(_cSock, (char*)&loginRes + sizeof(DataHeader), sizeof(LoginResult) - sizeof(DataHeader), 0);
        printf("recv server login res ,data len:%d\n", loginRes.dataLength);
    }
    break;
    case CMD_LOGOUT_RESULT:
    {
        LogoutResult logoutRes = {};
        recv(_cSock, (char*)&logoutRes + sizeof(DataHeader), sizeof(LogoutResult) - sizeof(DataHeader), 0);
        printf("recv server login res ,data len:%d\n", logoutRes.dataLength);
    }
    break;
    case CMD_NEW_USER_JOIN:
    {
        NewUserJoin newUser = {};
        recv(_cSock, (char*)&newUser + sizeof(DataHeader), sizeof(NewUserJoin) - sizeof(DataHeader), 0);
        printf("newuser join ,data len:%d\n", newUser.dataLength);
    }
        break;
    default:
        break;
    }
}

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
        fd_set fdReads;
        FD_ZERO(&fdReads);
        FD_SET(_sock, &fdReads);
        timeval t = {1, 0};
        int res = select(_sock + 1, &fdReads, NULL, NULL, &t);
        if (res < 0) {
            printf("select error\n");
            break;
        }

        if (FD_ISSET(_sock, &fdReads))
        {
            FD_CLR(_sock, &fdReads);
            if (-1 == processor(_sock))
            {
                printf("select error\n");
                break;
            }
        }
    }


    closesocket(_sock);
    WSACleanup();
    printf("client quit");
    getchar();
}