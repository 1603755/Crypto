#pragma once
#include <iostream>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <cstdio>

#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

class Utils
{
public:
	static std::pair<EVP_PKEY*, EVP_PKEY*> GetKeyRSApair();
	static std::vector<unsigned char> envelope_seal(EVP_PKEY** pub_key, unsigned char* plaintext, int plaintext_len, unsigned char** encrypted_key, int* encrypted_key_len, unsigned char* iv);
	static std::vector<unsigned char> envelope_open(EVP_PKEY* priv_key, unsigned char* ciphertext, int ciphertext_len, unsigned char* encrypted_key, int encrypted_key_len, unsigned char* iv);
	static std::string GetHex(std::vector<unsigned char> v);
	static std::vector<unsigned char> GetBinary(std::string s);
	static bool generate_key();
};
