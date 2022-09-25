#pragma once
#include <WinSock2.h>
#include "../../include/Coin.h"
#include "../include/Client.h"
#include "../include/Server.h"
#include "../../include/Thread.h"
class ConnectionManager {
public:
	ConnectionManager();

private:
	Coin coin;
	bool Running = true;
	bool Sync = false;
	uint32_t peer_id;
};
