#include "../include/Server.h"
Server::Server()
{
    // create socket
    int listen_sock = get_listen_socket();

    // set reuse
    set_reuse_addr(listen_sock);

    // bind
    listen_socket_bind(listen_sock);

    // listen
    listen_socket_listen(listen_sock);

    // socklen_t addr_size = sizeof(sockaddr_in);
    pollfd listen_pollfd;
    listen_pollfd.fd = listen_sock;
    listen_pollfd.events = POLLIN;
    poll_sets.push_back(listen_pollfd);

    while (1)
    {
        int ret = WSAPoll((pollfd*)&poll_sets[0], (unsigned int)poll_sets.size(), 150);

        if (ret <= 0)
        {
            // timeout is error
            perror("poll error");
            exit(1);
        }

        // check every pool in the set
        vector<pollfd>::iterator it;
        vector<pollfd>::iterator end = poll_sets.end();
        for (it = poll_sets.begin(); it != end; it++)
        {
            if (it->revents & POLLIN)
            {
                if (it->fd == listen_sock)
                {
                    // new client
                    int client_sock = accept_socket(listen_sock);
                    if (client_sock == -1)
                    {
                        perror("accept failed");
                        continue;
                    }

                    if (poll_sets.size() - 1 < MAX_CLIENT_NUM)
                    {
                        // add to pool set
                        add_client(client_sock);
                    }
                    else
                    {
                        too_many_clients(client_sock);
                    }
                }
                else
                {
                    // client echo message
                    char* message = new char[BUF_SIZE];
                    int read_size = recv(it->fd, message, BUF_SIZE, NULL);

                    if (read_size <= 0)
                    {
                        // client close
                        del_client(it);
                    }
                    else
                    {
                        //Send the message back to client
                        send(it->fd, message, strlen(message) + 1, NULL);
                    }
                    free(message);
                }
            }
            else if (it->revents & POLLERR)
            {
                if (it->fd == listen_sock)
                {
                    // listen socket error
                    perror("listen socket error");
                    exit(1);
                }
                else
                {
                    // client error
                    del_client(it);
                }
            }
        }
    }
}

void Server::add_client(int client_sock)
{
    pollfd client_pollfd;
    client_pollfd.fd = client_sock;
    client_pollfd.events = POLLIN;
    poll_sets.push_back(client_pollfd);
    printf("now_client_num: %u\n", (unsigned int)poll_sets.size() - 1);
}

void Server::del_client(vector<pollfd>::iterator it)
{
    poll_sets.erase(it);
    closesocket(it->fd);
    printf("now_client_num: %u\n", (unsigned int)poll_sets.size() - 1);
}