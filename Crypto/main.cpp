#include "Utils.h"
#include "Coin.h"
int main()
{
 /*
    auto keypair = Utils::GetKeyRSApair();

    unsigned char str[] = "I am encrypted4332048230948-2308402934702384-2384092384-0234-20384-2384-2384-234^&*(&(*&(*&9798";

    unsigned char iv[EVP_MAX_IV_LENGTH] = {};
    unsigned char* encrypted_key = (unsigned char*)malloc(EVP_PKEY_size(keypair.first));  //https://www.openssl.org/docs/man1.1.1/man3/EVP_SealInit.html
    int encrypted_key_len = EVP_PKEY_size(keypair.first);

    std::vector<unsigned char> cyphered = Utils::envelope_seal(&keypair.first, str, strlen((char*)str), &encrypted_key, &encrypted_key_len, iv);
    std::string cypheredString = Utils::GetHex(cyphered);
    printf("%s\n", cypheredString.c_str());


    std::vector<unsigned char> cypheredbinary = Utils::GetBinary(cypheredString);
    std::vector<unsigned char> plaintext = Utils::envelope_open(keypair.second, &cypheredbinary[0], cypheredbinary.size(), encrypted_key, encrypted_key_len, iv);
    printf("orgin text:%s:End\n", str);
    printf("plain text:");
    for (char c : plaintext)
        printf("%c", c);

    printf(":End\n");
    free(encrypted_key);
    EVP_PKEY_free(keypair.first); EVP_PKEY_free(keypair.second);
    
    Utils::generate_key();

    EVP_PKEY* pkey = Utils::getPublicKey();
    cout << "Address: " << Utils::getAddressFromPublicKey(keypair.first);
    EVP_PKEY_free(pkey);

    
    */

    

    Utils::Keys();
    cout <<"Bitcoin Address: " << Utils::getAddressFromPublicKey(Utils::getPublicKey()) << endl;
    uint256_t a;
    for (int i = 0; i < 32; i++)
        a.bits[i] = 1;
    Transaction tx(a, a, 10, 10);
    return 0;
}
