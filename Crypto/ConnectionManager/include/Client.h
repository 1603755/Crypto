#pragma once
#include "../../include/Utils.h"
#include <WinSock2.h>
#include "../../include/Block.h"
#include <queue>
#include "../../include/Coin.h"

class Client {
public:
	Client(Coin &coin);
	int connect_to_peer(SOCKADDR_IN peer);
	DWORD InitializeClientThread();
	void DiscoverPeers();
	Block deserializate_block(queue<uint8_t> block_data);
	CTransaction deserializate_transaction(queue<uint8_t> transaction_data);
	queue<uint8_t> serializate_socket_address(SOCKADDR_IN socket_addr);
private:
	std::vector<SOCKADDR_IN> peers;
	Coin* c;
};