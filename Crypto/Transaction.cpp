#include "Transaction.h"
 

Transaction::Transaction(uint256_t From, uint256_t To, uint32_t quantity, uint32_t time)
{
	//Initialize default transactin and do sign
	Timestamp = time;
	txFrom = From;
	txTo = To;
	amount = quantity;
	sign();
}
Transaction::Transaction(uint256_t From, uint256_t To, uint32_t quantity, vector<uint8_t> sign, uint32_t time)
{
	txFrom = From;
	txTo = To;
	amount = quantity;
	signature = sign;
	Timestamp = time;
}

vector<uint8_t> Transaction::getData() {
	//Return all data bytes
	string base64_txFrom = base64_encode(sha256::toString(txFrom.bits), false);
	string base64_txTo = base64_encode(sha256::toString(txTo.bits), false);
	string str(signature.begin(), signature.end());
	string bytesData = base64_txFrom + base64_txTo + to_string(amount) + to_string(Timestamp) + str;
	vector<uint8_t> bytesTx(bytesData.begin(), bytesData.end());
	return bytesTx;
}

bool Transaction::sign() {
	//Sign data of transaction object
    string message = sha256::toString(txFrom.bits) + sha256::toString(txTo.bits) + to_string(amount) + to_string(Timestamp);
    const char* c = const_cast<char*>(message.c_str());
    const unsigned char* text_to_sign = reinterpret_cast<const unsigned char*>(c);

    EC_KEY* key_pair_obj = nullptr;
	FILE* f = fopen("private.pem", "rt");
	PEM_read_ECPrivateKey(f, &key_pair_obj, NULL, NULL);
	
	EC_POINT* priv_key = (EC_POINT*)EC_KEY_get0_private_key(key_pair_obj);

	uint32_t signature_len;
	uint8_t* sign;
	uint8_t* digest;
	int	ret_error;
	unsigned char   buffer_digest[SHA256_DIGEST_LENGTH];

	signature_len = ECDSA_size(key_pair_obj);
	sign = (uint8_t*)OPENSSL_malloc(signature_len);

	//Generate the hash to sign
	digest = SHA256((const unsigned char*)text_to_sign, message.length(), buffer_digest);
	ret_error = ECDSA_sign(0, (const uint8_t*)digest, SHA256_DIGEST_LENGTH, sign, &signature_len, key_pair_obj);

	printf("Message SHA256: "); for (uint32_t i = 0; i < SHA256_DIGEST_LENGTH; i++) printf("%02x", digest[i]); printf("\n");
	printf("Signature     : "); for (uint32_t i = 0; i < signature_len; i++) printf("%02x", sign[i]); printf("\n");
	
	//We verify the signature
	int verification = ECDSA_verify(0, digest, SHA256_DIGEST_LENGTH, sign, signature_len, key_pair_obj);
	if (verification == 1)
		printf("Verification    successful\n");
	else
		printf("Verification    NOT successful\n");
		return false;
	EC_KEY_free(key_pair_obj);
	
	//Delete data signature if the object has
	while (signature.size() != 0) {
		signature.pop_back();
	}

	//Set the new signature to our buffer
	for (uint32_t i = 0; i < signature_len; i++) {
		signature.push_back(sign[i]);
	}

	return true;
}
