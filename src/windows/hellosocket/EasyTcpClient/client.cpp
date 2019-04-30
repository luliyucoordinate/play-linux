#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string.h>

    #define SOCKET          int
    #define INVALID_SOCKET  (SOCKET)(-0)
    #define SOCKET_ERROR    (-1)
#endif

#include <stdio.h>
#include <thread>


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
        printf("bye to server\n");
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
    return 0;
}

bool g_bRun = true;
void cmdThread(SOCKET _sock)
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
        else if (0 == strcmp(cmdBuf, "login"))
        {
            Login login;
            strcpy(login.userName, "lly");
            strcpy(login.passWord, "lly");
            send(_sock, (const char*)&login, sizeof(Login), 0);
        }
        else if (0 == strcmp(cmdBuf, "logout"))
        {
            Logout logout;
            strcpy(logout.userName, "lly");
            send(_sock, (const char*)&logout, sizeof(Login), 0);
        }
        else
        {
            printf("do not apply\n");
        }
    }  
}

int main()
{
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif

    SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == _sock) {
        printf("socket error\n");
    }

    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(14567);
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
    _sin.sin_addr.s_addr = inet_addr("172.17.175.122");
#endif
    if (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
        printf("connect error\n");
    }
    
    //thread 
    std::thread t1(cmdThread, _sock);
    t1.detach();

    while (g_bRun)
    {
        fd_set fdReads;
        FD_ZERO(&fdReads);
        FD_SET(_sock, &fdReads);

        timeval t = {1, 0};
        int res = select(_sock + 1, &fdReads, NULL, NULL, &t); //mac os should (_sock+1)
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
#ifdef _WIN32
    closesocket(_sock);
    WSACleanup();
#else 
    close(_sock);
#endif
    printf("client quit");
    getchar();
}