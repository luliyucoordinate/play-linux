#ifndef _MYTCPCLIENT_HPP_
#define _MYTCPCLIENT_HPP_

#ifdef _WIN32
    //for windows
    #define WIN32_LEAN_AND_MEAN
    #define FD_SETSIZE		1024
    #include <windows.h>
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    //for linux and mac os
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <string.h>
    #define SOCKET          int
    #define INVALID_SOCKET  (SOCKET)(-0)
    #define SOCKET_ERROR    (-1)
#endif

#include <stdio.h>
#include <vector>
#include <algorithm>
#include "MessageHeader.hpp"

#ifndef RECV_BUF_SIZE
#define RECV_BUF_SIZE 10240
#endif // !RECV_BUF_SIZE

class MyTcpClient
{
public:
    MyTcpClient() :
        _sock(INVALID_SOCKET),
        _lastPos(0)
    {
        memset(_szRecv, 0, RECV_BUF_SIZE);
        memset(_szMsgBuf, 0, RECV_BUF_SIZE*10);
    }
    virtual ~MyTcpClient()
    {
        Close();
    }

    //init socket
    int InitSocket()
    {
        //start socket on windows
#ifdef _WIN32
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif
        
        if (INVALID_SOCKET != _sock) 
        {
            printf("close old socket...\n");
            Close();
        }
        _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == _sock)
        {
            printf("establish socket fail!!!\n");
        }
        else
        {
            printf("establish socket<%d> finish!!!\n", _sock);
        }
        return 0;
    }

    //connect server
    int Connect(const char* ip, short port)
    {
        if (INVALID_SOCKET == _sock)
        {
            InitSocket();
        }
        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET;
        _sin.sin_port = htons(port);
#ifdef _WIN32
        _sin.sin_addr.S_un.S_addr = inet_addr(ip);
#else
        _sin.sin_addr.s_addr = inet_addr(ip);
#endif
        int res = connect(_sock, (sockaddr*)&_sin, sizeof(_sin));
        if (SOCKET_ERROR == res) 
        {
            printf("connect server:<%s:%d> error\n", ip, port);
        }
        else
        {
            printf("connect server:<%s:%d> finish\n", ip, port);
        }
        return res;
    }

    //close server
    void Close()
    {
        if (INVALID_SOCKET != _sock)
        {
#ifdef _WIN32
            closesocket(_sock);
            WSACleanup();
#else 
            close(_sock);
#endif
        }
        _sock = INVALID_SOCKET;
    }

    //receive message
    bool OnRun()
    {
        if (IsRun())
        {
            fd_set fdReads;
            FD_ZERO(&fdReads);
            FD_SET(_sock, &fdReads);

            timeval t = { 0, 0 }; //nonblock
            int res = select(_sock + 1, &fdReads, NULL, NULL, &t); //mac os should (_sock+1)
            if (res < 0)
            {
                printf("socket<%d> select error\n", _sock);
                Close();
                return false;
            }

            if (FD_ISSET(_sock, &fdReads))
            {
                FD_CLR(_sock, &fdReads);
                if (-1 == RecvData())
                {
                    printf("select error\n");
                    Close();
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    //receive data
    int RecvData()
    {
        //recv from client
        int nLen = recv(_sock, _szRecv, RECV_BUF_SIZE, 0);
        if (nLen <= 0)
        {
            printf("<socket=%d> stop connect\n", _sock);
            return -1;
        }
        //copy data from Message buf to receive buf
        memcpy(_szMsgBuf + _lastPos, _szRecv, nLen);
        _lastPos += nLen;

        while (_lastPos >= sizeof(DataHeader))
        {
            DataHeader* header = (DataHeader*)_szMsgBuf;
            if (_lastPos >= header->dataLength)
            {
                int nSize = _lastPos - header->dataLength;
                OnNetMsg(header);
                memcpy(_szMsgBuf, _szMsgBuf + header->dataLength, nSize);
                _lastPos = nSize;
            }
            else
            {
                //residual a message
                break;
            }
        }

        return 0;
    }

    //parse data
    virtual void OnNetMsg(DataHeader* header)
    {
        switch (header->cmd)
        {
        case CMD_LOGIN_RESULT:
        {
            LoginResult *loginRes = (LoginResult*)header;
            printf("recv data: login res, data len:%d\n", header->dataLength);
        }
        break;
        case CMD_LOGOUT_RESULT:
        {
            LogoutResult *logoutRes = (LogoutResult*)header;
            printf("recv data: login res, data len:%d\n", logoutRes->dataLength);
        }
        break;
        case CMD_NEW_USER_JOIN:
        {
            NewUserJoin *newUser = (NewUserJoin*)header;
            printf("recv data: newuser join, data len:%d\n", newUser->dataLength);
        }
        break;
        case CMD_ERROR:
        {
            printf("recv error, data len:%d\n", header->dataLength);
        }
        break;
        default:
        {
            printf("recv unknowned message, data len:%d\n", header->dataLength);
        }
        break;
        }
    }

    int SendData(DataHeader* header)
    {
        if (IsRun() && header)
        {
            return send(_sock, (const char*)header, header->dataLength, 0);
        }
        return SOCKET_ERROR;
    }

    bool IsRun()
    {
        return _sock != INVALID_SOCKET;
    }
private:
    SOCKET _sock;
    int _lastPos;
    char _szRecv[RECV_BUF_SIZE];
    char _szMsgBuf[RECV_BUF_SIZE * 10];
};
#endif // _MYTCPCLIENT_HPP_
