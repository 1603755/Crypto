#include "Transaction.h"
 

Transaction::Transaction(uint256_t From, uint256_t To, uint32_t quantity, uint32_t time)
{
	Timestamp = time;
	txFrom = From;
	txTo = To;
	amount = quantity;
}
Transaction::Transaction(uint256_t From, uint256_t To, uint32_t quantity, vector<uint8_t> *sign, uint32_t time)
{
	txFrom = From;
	txTo = To;
	amount = quantity;
	signature = sign;
	Timestamp = time;
}

//FALTA ACABAR
vector<uint8_t> Transaction::getData() {
	string base64_txFrom = base64_encode(sha256::toString(txFrom.bits), false);
	string base64_txTo = base64_encode(sha256::toString(txTo.bits), false);
	string str(signature->begin(), signature->end());
	string bytesData = base64_txFrom + base64_txTo + to_string(amount) + to_string(Timestamp) + str;
	vector<uint8_t> bytesTx(bytesData.begin(), bytesData.end());
	return bytesTx;
}

bool Transaction::sign(uint256_t PrivateKey) {
	if ((*signature).size() != 0) //La transaccion ya esta firmada
		return false;

	
}