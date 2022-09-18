#pragma once
#include "../../include/Utils.h"
#include <WinSock2.h>

class Client {
public:
	Client(uint32_t peer_id);
private:
	uint32_t peer_id;
};