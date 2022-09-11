#pragma once
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Wallet
{
public:
	Wallet();
	~Wallet();
	void genKeyPair();

private:
	char* walletPrivateKey;
	char* walletPublicKey;
};

Wallet::Wallet()
{
}

Wallet::~Wallet()
{
}