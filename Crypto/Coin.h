#pragma once
#include <queue>
#include "Block.h"
using namespace std;
class Coin
{
public:
	Coin();
	uint8_t DIFFICULTY = 0;
	uint8_t MAX_FEE;
	uint8_t PORT;
	uint8_t timeout;

	Block getLastBlockOnChain() { if (BlockChain.size() != 0) { return BlockChain[BlockChain.size() - 1]; } else { return Block(); } }
	void saveBlockchainAsJSON();
	bool addBlockToChain(Block block);
	bool addCandidateBlock(Block block);
	//Metodo para reajustar la dificultad
	void readjustDifficulty();
private:
	vector<Block> BlockChain;
	queue<Transaction> pendingTX;
	queue<Block> candidates;
};
