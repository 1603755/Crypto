#include "../include/ConnectionManager.h"

ConnectionManager::ConnectionManager(Coin c) {
	coin = c;
	srand(time(NULL));
	peer_id = rand() % 0xFFFFFFFF; //2^32
	Client client(peer_id);
	Server s;
}

/*SERVER*/

void ConnectionManager::ServerAccept() {
	
}