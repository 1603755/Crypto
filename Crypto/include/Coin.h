#pragma once
#include <queue>
#include "../include/Block.h"
#include <string>



//#include <WinSock.h> sockets
using namespace std;

class Coin
{
public:
	Coin();
	uint8_t DIFFICULTY = 0;
	uint32_t REWARD_BLOCK = 6.25;
	uint8_t MAX_FEE;
	uint16_t PORT = 10000;
	uint8_t timeout;
	//DONE
	Block getLastBlockOnChain() { if (BlockChain.size() != 0) { return BlockChain[BlockChain.size() - 1]; } else { return Block(); } }
	bool saveBlockChain();
	
	//TO DO
	bool loadBlockChain();
	void readjustReward();
	
	//DONE
	void saveBlockchainAsJSON();
	bool addBlockToChain(Block block);
	bool addCandidateBlock(Block block);
	//Metodo para reajustar la dificultad
	void readjustDifficulty();
	vector<Block> BlockChain;
	Block getBlock(uint64_t num);
	float getBalance(uint256_t PubKey);
private:
	queue<CTransaction> pendingTX;
	queue<Block> candidates;
};
