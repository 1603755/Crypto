#ifndef _BASE_H
#define _BASE_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;

int get_listen_socket();
void set_reuse_addr(int listen_sock);
void listen_socket_bind(int listen_sock);
void listen_socket_listen(int listen_sock);
int accept_socket(int listen_sock);
SOCKADDR_IN get_client_addr(int sock);
void too_many_clients(int client_sock);
#endif