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
        else if (0 == strcmp(cmdBuf, "login")) {
            Login login{};
            strcpy(login.userName, "lly");
            strcpy(login.passWord, "lly");
            send(_sock, (const char*)&login, sizeof(login), 0);
            //recv from server
            LoginResult loginret = {};
            recv(_sock, (char*)&loginret, sizeof(LoginResult), 0);
            printf("LoginResult: %d\n", loginret.res);
        }
        else if (0 == strcmp(cmdBuf, "logout")) {
            Logout logout = {};
            strcpy(logout.userName, "lly");
            DataHeader dh = { sizeof(Logout),CMD_LOGOUT };
            //向服务器发送请求命令
            send(_sock, (const char*)&logout, sizeof(logout), 0);
            //接受服务器返回的数据
            LogoutResult logoutRet = {};
            recv(_sock, (char*)&logoutRet, sizeof(LogoutResult), 0);
            printf("LoginResult: %d\n", logoutRet.res);
        }
        else {
            printf("unknowned cmd\n");
        }
    }


    closesocket(_sock);
    WSACleanup();
    printf("client quit");
    getchar();
}