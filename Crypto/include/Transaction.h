#include "../include/base64.h"
#include <ctime>
#include <cstdint>
#include <vector>
#include "../include/sha256.h"
#include "../include/Utils.h"
using namespace std;


class Transaction
{
public:
	Transaction(uint256_t From, uint256_t To, uint32_t quantity, uint32_t time);
	Transaction(uint256_t From, uint256_t To, uint32_t quantity, vector<uint8_t> sign, uint32_t time);

	void operator=(Transaction tx) { txFrom = tx.txFrom; txTo = tx.txTo; amount = tx.amount; Timestamp = tx.Timestamp; signature = tx.signature; }

	bool sign();
	uint256_t getTxFrom() { return txFrom; }
	uint256_t getTxTo() { return txTo; }
	uint32_t getAmount() { return amount; }
	time_t getTime() { return Timestamp; }

	uint256_t txFrom;
	uint256_t txTo;
	uint32_t amount;
	uint32_t Timestamp;
	vector<uint8_t> signature;
};

