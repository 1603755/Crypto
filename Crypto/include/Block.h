#pragma once
#include <time.h>
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
	uint32_t getDate() { return date; }
	uint16_t getNumTX() { return num_tx; }
	uint32_t getNumBytes() { return nbytes; };

	uint32_t nbytes;
	uint256_t hash;
	uint256_t prevHash;
	uint256_t hashMerkleRoot;
	uint32_t height;
	uint32_t nonce;
	uint32_t date;
	uint16_t num_tx;
};


class Block : public BlockHeader
{
public:
	vector<CTransaction> tx; 
	Block();
	Block(uint32_t bytes, uint256_t Hash, uint256_t prev, uint256_t hashMerkle, uint32_t h, uint32_t Nonce, uint32_t time, uint16_t nTransactions, vector<Transaction> vtx);
	void mine(uint8_t diff);
	bool verifyHash(string hash, uint8_t diff);
	
	vector<CTransaction> getTX() { return tx; }
	//no serveix crec
	bool operator ==(Block b) { return(height == b.height && nonce == b.nonce && Utils::toString(hash.bits) == Utils::toString(b.hash.bits) && Utils::toString(prevHash.bits) == Utils::toString(b.prevHash.bits) && Utils::toString(hashMerkleRoot.bits) == Utils::toString(b.hashMerkleRoot.bits)); }
	void operator =(Block b) { height = b.height; nonce = b.nonce;  Utils::toString(hash.bits) = Utils::toString(b.hash.bits); Utils::toString(prevHash.bits) = Utils::toString(b.prevHash.bits); Utils::toString(hashMerkleRoot.bits) = Utils::toString(b.hashMerkleRoot.bits); tx = b.tx; }
};
