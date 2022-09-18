#include "base.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include  <sys/types.h>
#include <errno.h>
#include <vector>
#include "../../include/Coin.h"
#include "../../include/Utils.h"

extern const int BUF_SIZE;
extern const char SERVER_IP[];
extern const int SERVER_PORT;
extern const int BACKLOG;
extern const unsigned int MAX_CLIENT_NUM;


class Server 
{
public:
	Server();
	void add_client(int client_sock);
	void del_client(std::vector<pollfd>::iterator it);
private:
	std::vector<pollfd> poll_sets;
	int listen_sockfd;
	Coin *c;
};