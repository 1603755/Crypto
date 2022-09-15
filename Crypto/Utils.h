#pragma once
#include <iostream>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/obj_mac.h>

#include <openssl/ec.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <cstring>

# include "sha256.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

struct uint256_t {
	std::uint8_t bits[32];
	void operator= (uint8_t* num) { for (uint8_t i = 0; i < sizeof(bits); i++) { bits[i] = num[i]; } };
};


class Utils
{ 
public:
	static std::vector<unsigned char> envelope_seal(EVP_PKEY** pub_key, unsigned char* plaintext, int plaintext_len, unsigned char** encrypted_key, int* encrypted_key_len, unsigned char* iv);
	static std::vector<unsigned char> envelope_open(EVP_PKEY* priv_key, unsigned char* ciphertext, int ciphertext_len, unsigned char* encrypted_key, int encrypted_key_len, unsigned char* iv);
	static std::string GetHex(std::vector<unsigned char> v);
	static std::vector<unsigned char> GetBinary(std::string s);
	static std::string getAddressFromPublicKey(std::string PK);
	static std::string getPublicKey();
	static void Keys();
	static std::string hexToASCII(std::string &hex);
	static std::string ASCIItoHEX(std::string& ascii);
};
