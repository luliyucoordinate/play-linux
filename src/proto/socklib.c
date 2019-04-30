#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define BACKLOG 1
#define HOSTLEN 256
#define oops(msg) { perror(msg); exit(1); }

int make_server_socket_q(int, int);
int make_server_socket(int portnum)
{
    return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog)
{
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    int sock_id = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) return -1;
    memset(&saddr, 0, sizeof saddr);

    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;

    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof saddr) != 0) -1;
    if (listen(sock_id, 1) != 0) -1;
    return sock_id;
}

int connect_to_server(char *host, int portnum)
{
    struct sockaddr_in servadd;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return -1;

    memset(&servadd, 0, sizeof servadd);

    servadd.sin_addr.s_addr = inet_addr("127.0.0.1");
    servadd.sin_port = htons(portnum);
    servadd.sin_family = AF_INET;

    if (connect(sock, (struct sockaddr*)&servadd, sizeof servadd) != 0) return -1;
    return sock;
}