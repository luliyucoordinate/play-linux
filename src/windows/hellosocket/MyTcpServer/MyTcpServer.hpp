#ifndef _MYTCPSERVER_HPP_
#define _MYTCPSERVER_HPP_

#ifdef _WIN32
    //for windows
    #define WIN32_LEAN_AND_MEAN
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



class MyTcpServer
{
public:
    MyTcpServer() :
        _sock(INVALID_SOCKET)
    {

    }
    virtual ~MyTcpServer()
    {
        Close();
    }

    //init socket
    SOCKET InitSocket()
    {
        //start socket on windows
#ifdef _WIN32
        WORD ver = MAKEWORD(2, 2);
        WSADATA dat;
        WSAStartup(ver, &dat);
#endif
        
        if (INVALID_SOCKET == _sock) 
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
        return _sock;
    }

    //bind port 
    int Bind(const char* ip, short port)
    {
        if (INVALID_SOCKET == _sock)
        {
            InitSocket();
        }

        sockaddr_in _sin = {};
        _sin.sin_family = AF_INET;
        _sin.sin_port = htons(14567);

#ifdef _WIN32
        if (ip)
        {
            _sin.sin_addr.S_un.S_addr = inet_addr(ip);
        }
        else
        {
            _sin.sin_addr.S_un.S_addr = INADDR_ANY;
        }
#else
        if (ip)
        {
            _sin.sin_addr.s_addr = inet_addr(ip);
        }
        else
        {
            _sin.sin_addr.s_addr = INADDR_ANY;

        }
#endif
        int res = bind(_sock, (sockaddr*)&_sin, sizeof _sin);
        if (SOCKET_ERROR == res) 
        {
            printf("bind port<%d> error...\n", port);
        }
        else
        {
            printf("bind port<%d> finish...\n", port);
        }
        return res;
    }

    //close server
    void Close()
    {
        if (INVALID_SOCKET != _sock)
        {
#ifdef _WIN32
            for (size_t i = 0; i < g_clients.size(); ++i)
            {
                closesocket(g_clients[i]);
            }
            closesocket(_sock);
            WSACleanup();
#else 
            for (size_t i = 0; i < g_clients.size(); ++i)
            {
                close(g_clients[i]);
            }
            close(_sock);
#endif
            _sock = INVALID_SOCKET;
        }
    }

    int Listen(int n)
    {
        int res = listen(_sock, n);
        if (SOCKET_ERROR == res)
        {
            printf("socket<%d> listen error...\n", (int)_sock);
        }
        else
        {
            printf("socket<%d> listen port finish...\n", (int)_sock);
        }
        return res;
    }

    SOCKET Accept()
    {
        sockaddr_in clientAddr = {};
        int nAddrLen = sizeof(sockaddr_in);
#ifndef _WIN32
        SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr, (socklen_t *)&nAddrLen);
#else
        SOCKET _cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
#endif
        if (INVALID_SOCKET == _cSock)
        {
            printf("socket<%d> error, none client!\n", (int)_cSock);
        }
        else
        {
            NewUserJoin userJoin;
            SendDataToAll(&userJoin);
            g_clients.push_back(_cSock);
            printf("new client<%d> :IP=%s\n", int(_cSock), inet_ntoa(clientAddr.sin_addr));
        }
        return _cSock;
    }

    //receive message
    bool OnRun()
    {
        if (IsRun())
        {
            fd_set fdRead, fdWrite, fdExcept;

            FD_ZERO(&fdRead);
            FD_ZERO(&fdWrite);
            FD_ZERO(&fdExcept);

            FD_SET(_sock, &fdRead);
            FD_SET(_sock, &fdWrite);
            FD_SET(_sock, &fdExcept);

            SOCKET maxSock = _sock;

            for (size_t i = 0; i < g_clients.size(); ++i)
            {
                FD_SET(g_clients[i], &fdRead);
                maxSock = max(maxSock, (int)g_clients[i]);
            }

            timeval t = { 0, 0 };
            int res = select(maxSock + 1, &fdRead, &fdWrite, &fdExcept, &t);
            if (res < 0)
            {
                printf("select error\n");
                Close();
                return false;
            }

            if (FD_ISSET(_sock, &fdRead))
            {
                FD_CLR(_sock, &fdRead);
                Accept();
            }
            for (size_t i = 0; i < g_clients.size(); ++i)
            {
                if (FD_ISSET(g_clients[i], &fdRead))
                {
                    if (-1 == RecvData(g_clients[i]))
                    {
                        auto iter = g_clients.begin() + i;
                        if (iter != g_clients.end())
                        {
                            g_clients.erase(iter);
                        }
                    }
                }
            }
            return true;
        }
        return false;
    }

    //receive data
    int RecvData(SOCKET _cSock)
    {
        char recvBuf[4096] = {};
        //recv from client
        int nLen = (int)recv(_cSock, recvBuf, sizeof(DataHeader), 0);
        DataHeader *header = (DataHeader*)recvBuf;
        if (nLen <= 0) 
        {
            printf("client<Socket=%d> quit\n", _cSock);
            return -1;
        }
        recv(_cSock, recvBuf + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
        OnNetMsg(_cSock, header);
        return 0;
    }

    //parse data
    virtual void OnNetMsg(SOCKET _cSock, DataHeader* header)
    {
        switch (header->cmd)
        {
        case CMD_LOGIN:
        {
            Login *login = (Login*)header;
            printf("recv cmd_login ,data len:%d, userName=%s, passwd=%s\n", login->dataLength, login->userName, login->passWord);
            LoginResult res;
            send(_cSock, (char*)&res, sizeof(LoginResult), 0);
        }
        break;
        case CMD_LOGOUT:
        {
            Logout *logout = (Logout*)header;
            printf("recv cmd_login ,data len:%d, userName=%s\n", logout->dataLength, logout->userName);
            LogoutResult res;
            send(_cSock, (char*)&res, sizeof(LogoutResult), 0);
        }
        break;
        default:
            header->cmd = CMD_ERROR;
            header->dataLength = 0;
            send(_cSock, (char*)&header, sizeof(header), 0);
            break;
        }
    }

    //send speckfy
    int SendData(SOCKET _cSock, DataHeader* header)
    {
        if (IsRun() && header)
        {
            return send(_cSock, (const char*)header, header->dataLength, 0);
        }
        return SOCKET_ERROR;
    }

    //send all
    int SendDataToAll(DataHeader* header)
    {
        if (IsRun() && header)
        {
            for (int i = 0; i < g_clients.size(); ++i)
            {
                SendData(g_clients[i], header);
            }
        }
        return SOCKET_ERROR;
    }

    bool IsRun()
    {
        return _sock != INVALID_SOCKET;
    }
private:
    SOCKET _sock;
    std::vector<SOCKET> g_clients;
};
#endif // _MYTCPSERVER_HPP_
