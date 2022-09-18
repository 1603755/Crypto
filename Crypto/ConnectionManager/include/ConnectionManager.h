#pragma once
#include <WinSock2.h>
#include "../../include/Coin.h"
#include "../include/Client.h"
#include "../include/Server.h"

class ConnectionManager {
public:
	ConnectionManager(Coin c);
	void ServerAccept();
private:
	Coin coin;
	vector<IN_ADDR> peers;
	bool Running = true;
	bool Sync = false;
	uint32_t peer_id;
};
