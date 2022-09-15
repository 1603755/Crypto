#pragma once
#include <time.h>
#include "sha256.h"
#include "Transaction.h"


class BlockHeader 
{
public:
	BlockHeader ();
	uint32_t getHeight() { return height; }
	uint32_t getNonce() { return nonce; }
	uint256_t getHash() { return hash; }
	uint256_t getPrevHash() { return prevHash; }
	uint256_t getHashMerkleRoot() { return hashMerkleRoot; }
	time_t getDate() { return date; }

	uint32_t height;
	uint32_t nonce;
	uint256_t hash;
	uint256_t prevHash;
	uint256_t hashMerkleRoot;
	time_t date;
};


class Block : public BlockHeader
{
public:
	vector<Transaction> tx;
	Block();
	Block(uint32_t h, uint32_t nonc, uint256_t prev, uint256_t hashMerkle, vector<Transaction> vtx, time_t time);
	void mine(uint8_t diff);
	bool verifyHash(string hash, uint8_t diff);
	
	vector<Transaction> getTX() { return tx; }
	//no serveix crec
	bool operator ==(Block b) { return(height == b.height && nonce == b.nonce && sha256::toString(hash.bits) == sha256::toString(b.hash.bits) && sha256::toString(prevHash.bits) == sha256::toString(b.prevHash.bits) && sha256::toString(hashMerkleRoot.bits) == sha256::toString(b.hashMerkleRoot.bits)); }
};
