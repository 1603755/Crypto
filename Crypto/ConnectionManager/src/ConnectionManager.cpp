#include "../include/ConnectionManager.h"

ConnectionManager::ConnectionManager() {
	Coin c;
	c.loadBlockChain();
	coin = c;
	srand(time(NULL));
	peer_id = rand() % 0xFFFFFFFF; //2^32

	Client client(c);
	Thread<Client> threadClient(&client, &Client::InitializeClientThread);
	if (threadClient.start())
		std::cout << "Client thread started correctly!" << std::endl;

	Server server(c);
	Thread<Server> threadServer(&server, &Server::InitializeServerThread);
	if (threadServer.start())
		std::cout << "Server thread started correctly!" << std::endl;
}
