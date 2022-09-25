#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/Utils.h"

class Wallet
{
public:
	Wallet();
	~Wallet();

private:
	std::string PublicKey;
};
