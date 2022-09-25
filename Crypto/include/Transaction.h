#include "../include/base64.h"
#include <ctime>
#include <cstdint>
#include <vector>
#include "../include/Utils.h"
using namespace std;

/** An outpoint - a combination of a transaction hash and an index n into its vout */

class COutPoint {
	uint256_t hash;
	uint32_t index;
};

class CTxIn {
	COutPoint outpoint;
	uint32_t sequence;
	uint8_t* signature;
};

class CTxOut {
	uint64_t value;
	uint32_t pk_script_length;
	uint8_t* pk_script;
};

class CTransaction
{
public:
	CTransaction(const CMutableTransaction& tx) : vin(tx.vin), vout(tx.vout), nVersion(tx.nVersion), timestamp(tx.timestamp), hash{ ComputeHash()} {}
	CTransaction(const CMutableTransaction&& tx) : vin(std::move(tx.vin)), vout(std::move(tx.vout)), nVersion(tx.nVersion), timestamp(tx.timestamp), hash{ ComputeHash() } {}
	const std::vector<CTxOut> vout;
	const std::vector<CTxIn> vin;
	const uint32_t timestamp;
	const int32_t nVersion;
private:
	vector<uint8_t> signature;
	const uint256_t hash;
	uint256_t ComputeHash() const;
	bool sign();
};

template <typename Stream, typename	TxType>
inline void serializeTransaction(const TxType& tx, Stream& s) {
	s << tx.vin;
	s << tx.vout;
	s << tx.timestamp;
}

template <typename Stream, typename TxType>
inline void UnserializeTransaction(TxType& tx, Stream& s) {
	tx.vin.clear();
	tx.vout.clear();
	s >> tx.vin;
	s >> tx.vout;
	s >> tx.timestamp;
}

struct CMutableTransaction {
	std::vector<CTxIn> vin;
	std::vector<CTxOut> vout;
	int32_t nVersion;
	uint32_t timestamp;
};