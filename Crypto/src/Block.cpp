#include "../include/Block.h"

BlockHeader::BlockHeader()
{
	height = NULL;
	nonce = NULL;
}

Block::Block() {
	uint256_t null_hash;
	for (int i = 0; i < 32; i++) {
		null_hash.bits[i] = 0;
	}
	hash = null_hash;
	hashMerkleRoot = null_hash;
	prevHash = null_hash;
	date = NULL;
	height = NULL;
	nonce = NULL;
	date = NULL;
}
Block::Block(uint32_t Bytes, uint256_t Hash, uint256_t prev, uint256_t hashMerkle, uint32_t h, uint32_t Nonce, uint32_t time, uint16_t nTransactions, vector<Transaction> vtx)
{
	nbytes = Bytes;
	hash = Hash;
	prevHash = prev;
	hashMerkleRoot = hashMerkle;
	height = h;
	nonce = Nonce;
	date = time;
	num_tx = nTransactions;
	tx = vtx;
	

}

void Block::mine(uint8_t diff) {
	srand(time(NULL));
	uint32_t nNonce = rand() % 0xFFFFFFFF;
	string signature = to_string(height) + to_string(nNonce) + Utils::toString(prevHash.bits) + Utils::toString(hashMerkleRoot.bits);
	uint256_t h;
	SHA256((unsigned char*)&signature, signature.length(), h.bits);
	
	while (!verifyHash(Utils::toString(h.bits), diff)) {
		//numero aleatorio entre 0 i 2^32 - 1
		nNonce = rand() % 0xFFFFFFFF;
		signature = to_string(height) + to_string(nNonce) + Utils::toString(prevHash.bits) + Utils::toString(hashMerkleRoot.bits);
		SHA256((unsigned char*)&signature, signature.length(), h.bits);
	}
	date = time(NULL);
	nonce = nNonce;
	hash = h.bits;
}

bool Block::verifyHash(string hash, uint8_t diff) {
	bool verify = false;
	int count = 0;
	if (diff <= hash.length()) {
		for (int i = 0; i < diff; i++) {
			if (hash[i] == '0') {
				count++;
			}
			else {
				break;
			}
		}
	}
	if (count == diff) {
		uint256_t h;
		string signature = to_string(height) + to_string(nonce) + Utils::toString(prevHash.bits) + Utils::toString(hashMerkleRoot.bits);
		SHA256((unsigned char*)&signature, signature.length(), h.bits);
		if ((uint8_t*)&hash == h.bits)
			verify = true;
	}
	return verify;
}