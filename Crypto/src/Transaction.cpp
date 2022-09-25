#include "../include/Transaction.h"
 

bool CTransaction::sign() {
	vector<const unsigned char> to_sign_data;
	serializeTransaction(this, to_sign_data);

	
    EC_KEY* key_pair_obj = nullptr;
	FILE* f = fopen(PRIVATE_KEY_PATH, "rt");
	PEM_read_ECPrivateKey(f, &key_pair_obj, NULL, NULL);
	fclose(f);
	EC_POINT* priv_key = (EC_POINT*)EC_KEY_get0_private_key(key_pair_obj);

	uint32_t signature_len;
	uint8_t* sign;
	uint8_t* digest;
	int	ret_error;
	unsigned char   buffer_digest[SHA256_DIGEST_LENGTH];

	signature_len = ECDSA_size(key_pair_obj);
	sign = (uint8_t*)OPENSSL_malloc(signature_len);

	//Generate the hash to sign
	digest = SHA256((const unsigned char *)&to_sign_data, to_sign_data.size(), buffer_digest);
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

uint256_t CTransaction::ComputeHash() const
{
	vector<uint8_t> serializated_data;
	serializeTransaction(*this, serializated_data);
	uint256_t transaction_hash;
	SHA256((uint8_t *)&serializated_data, serializated_data.size(), transaction_hash.bits);
	return transaction_hash;
}


