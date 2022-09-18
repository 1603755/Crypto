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

#define PRIVATE_KEY_PATH "ec-keys/ec-private.pem"
#define PUBLIC_KEY_PATH "ec-keys/ec-public.pem"
#define BLOCKCHAIN_DATA_PATH "data/block"

struct uint256_t {
	std::uint8_t bits[32];
	inline void operator= (uint8_t* num) { for (uint8_t i = 0; i < sizeof(bits); i++) { bits[i] = num[i]; } };
	inline bool operator== (uint8_t* num) { for (uint8_t i = 0; i < sizeof(bits); i++) { if (bits[i] != num[i]) { return false; } } return true; };
};


class Utils
{ 
public:
	static std::vector<unsigned char> GetBinary(std::string s);
	static std::string getAddressFromPublicKey(std::string PK);
	static std::string getPublicKey();
	static void Keys();
	static std::string hexToASCII(std::string &hex);
	static std::string ASCIItoHEX(std::string& ascii);
	static bool file_exists(const std::string& name);
};
