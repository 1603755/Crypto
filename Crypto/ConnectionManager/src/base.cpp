#include "../include/base.h"
#include <string>;
typedef int socklen_t;

extern const int BUF_SIZE = 1024;
extern const char SERVER_IP[] = "0.0.0.0";
extern const int SERVER_PORT = 8888;
extern const int BACKLOG = 3;

extern const unsigned int MAX_CLIENT_NUM = 100 * 1024 + 10;
extern const char ERR_MESSAGE[] = "[ERR]too many clients!!!";
extern const int CLIENT_INTERVAL = 1;

int get_listen_socket()
{
    int listen_sock;
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1)
    {
        perror("get_listen_socket error");
        exit(1);
    }
    return listen_sock;
}

void set_reuse_addr(int listen_sock)
{
    linger lin;
    lin.l_onoff = 0;
    lin.l_linger = 0;
    const int enable = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&lin, sizeof(int)) == -1)
    {
        perror("set_reuse_addr error");
        exit(1);
    }
}

void listen_socket_bind(int listen_sock)
{
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(listen_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("listen_socket_bind error");
        exit(1);
    }
}

void listen_socket_listen(int listen_sock)
{
    if (listen(listen_sock, BACKLOG) == -1)
    {
        perror("listen_socket_listen error");
        exit(1);
    }
}

int accept_socket(int listen_sock)
{
    sockaddr_in client;
    socklen_t addr_size = sizeof(SOCKADDR_IN);
    return accept(listen_sock, (sockaddr*)&client, &addr_size);
}

sockaddr_in get_client_addr(int sock)
{
    sockaddr_in client;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    if (getpeername(sock, (struct sockaddr*)&client, &addr_size) != 0)
    {
        perror("get_client_addr error");
        exit(1);
    }
    return client;
}

void too_many_clients(int client_sock)
{
    printf("%s\n", ERR_MESSAGE);
    send(client_sock, ERR_MESSAGE, strlen(ERR_MESSAGE) + 1, 0);
    closesocket(client_sock);
}

void print_start()
{
    printf("Server started success, listen at %s:%d\n", SERVER_IP, SERVER_PORT);
}

