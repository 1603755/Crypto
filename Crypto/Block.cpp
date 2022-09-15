#include "Block.h"

BlockHeader::BlockHeader()
{
	height = NULL;
	nonce = NULL;
}

Block::Block() {
	height = NULL;
	nonce = NULL;
	date = NULL;
}
Block::Block(uint32_t h, uint32_t nonc, uint256_t prev, uint256_t hashMerkle, vector<Transaction> vtx, time_t time)
{
	height = h;
	nonce = nonc;
	prevHash = prev;
	hashMerkleRoot = hashMerkle;
	tx = vtx;
	date = time;

}

void Block::mine(uint8_t diff) {
	srand(time(NULL));
	uint64_t nNonce = rand() % 0xFFFFFFFF;
	sha256 sha;
	string signature = to_string(height) + to_string(nNonce) + sha256::toString(prevHash.bits) + sha256::toString(hashMerkleRoot.bits);
	sha.update(signature);
	uint256_t h;
	h = sha.digest();
	while (!verifyHash(sha256::toString(h.bits), diff)) {
		//numero aleatorio entre 0 i 2^32 - 1
		nNonce = rand() % 0xFFFFFFFF;
		signature = to_string(height) + to_string(nNonce) + sha256::toString(prevHash.bits) + sha256::toString(hashMerkleRoot.bits);
		sha.update(signature);
		h = sha.digest();
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
		string signature = to_string(height) + to_string(nonce) + sha256::toString(prevHash.bits) + sha256::toString(hashMerkleRoot.bits);
		sha256 sha;
		sha.update(signature);
		string h = sha256::toString(sha.digest());
		if (hash == h)
			verify = true;
	}
	return verify;
}