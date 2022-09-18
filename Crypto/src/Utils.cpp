//https://wiki.openssl.org/index.php/EVP_Asymmetric_Encryption_and_Decryption_of_an_Envelope
#include "../include/Utils.h"
#include "../include/base58.h"

using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;

string Utils::getAddressFromPublicKey(string PK) {
    //This generates our address from our Public Key
    unsigned char* in = new unsigned char[65];
    for (uint8_t i = 0; i < 65; i++) {
        in[i] = PK[i];
    }
    //First we do SHA_256 hash to our publickey, then we generate another one using RIPEMD160, resulting as 20 bytes of hash, finally we add one extra byte cause we are on Mainnet.
    uint8_t rmd[RIPEMD160_DIGEST_LENGTH + 1];
    RIPEMD160(SHA256(in, 65, 0), 21, rmd+1);
    rmd[0] = 0x00;

    //Here we need to generate a checksum to validate the integity of our address
    unsigned char checksum[32];
    SHA256(SHA256(rmd, 21, 0), SHA256_DIGEST_LENGTH, checksum);

    //We add the checksum at the end of the 21 bytes hash that we generated previously.
    vector<unsigned char> aux;
    for (uint8_t i = 0; i < RIPEMD160_DIGEST_LENGTH + 1; i++) {
        aux.push_back(rmd[i]);
    }
    for (uint8_t i = RIPEMD160_DIGEST_LENGTH + 1; i < RIPEMD160_DIGEST_LENGTH + 1 + 4; i++) {
        aux.push_back(checksum[i - RIPEMD160_DIGEST_LENGTH + 1]);
    }

    //Encode to base 58
    CodecMapping mapping(AlphaMap, Base58Map);
    string encoded_data;
    encoded_data = base58::Base58Encode(aux, mapping);
    uint8_t i = 0;
    while (encoded_data[i] == '1') {
        i++;
    }
    i--;

    //Abracadabra here we got our Address
    string address = encoded_data.substr(i, encoded_data.length());
    return address;
}

std::string Utils::getPublicKey() {
    //Load our public key from .pem file
    EC_KEY* key_pair_obj = nullptr;
    FILE* file = fopen(PUBLIC_KEY_PATH, "rt");
    PEM_read_EC_PUBKEY(file, &key_pair_obj, NULL, NULL);

    char* pub_key_char;
    EC_POINT* pub_key;
    EC_GROUP* secp256k1_group;

    pub_key = (EC_POINT*)EC_KEY_get0_public_key(key_pair_obj);
    secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    pub_key_char = EC_POINT_point2hex(secp256k1_group, pub_key, POINT_CONVERSION_UNCOMPRESSED, nullptr);

    return pub_key_char;
}

void Utils::Keys()
{
    //Generates our private and public keys
    EC_KEY* key_pair_obj = nullptr;;
    BIGNUM* priv_key;
    EC_POINT* pub_key;
    EC_GROUP* secp256k1_group;

    char* pub_key_char;
    char* priv_key_char;

    int ret_error;

    // Generate secp256k1 key pair
    key_pair_obj = EC_KEY_new_by_curve_name(NID_secp256k1);
    ret_error = EC_KEY_generate_key(key_pair_obj);

    // Get private key
    priv_key = (BIGNUM*)EC_KEY_get0_private_key(key_pair_obj);
    priv_key_char = BN_bn2hex(priv_key);
 
    // Get public key
    pub_key = (EC_POINT*)EC_KEY_get0_public_key(key_pair_obj);
    secp256k1_group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    pub_key_char = EC_POINT_point2hex(secp256k1_group, pub_key, POINT_CONVERSION_UNCOMPRESSED, nullptr);

    EC_GROUP_free(secp256k1_group);

    printf("Private key: %s\n", priv_key_char);
    printf("Public key : %s\n", pub_key_char);

    //string hex_priv = priv_key_char;
   // std::string ascii_priv = hexToASCII(hex_priv);

    FILE* PrivKey;
    PrivKey = fopen(PRIVATE_KEY_PATH, "w");
    PEM_write_ECPrivateKey(PrivKey,key_pair_obj, NULL, NULL, NULL, NULL, NULL);
    fclose(PrivKey);

    string hex_pub = pub_key_char;
    std::string ascii_pub = hexToASCII(hex_pub);

    FILE* PubKey;
    PubKey = fopen(PUBLIC_KEY_PATH, "w");
    PEM_write_EC_PUBKEY(PubKey, key_pair_obj);
    fclose(PubKey);
}

int hex_value(unsigned char hex_digit)
{
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::invalid_argument("invalid hex digit");
    return value;
}

std::string Utils::hexToASCII(string &hex)
{
    const auto len = hex.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (auto it = hex.begin(); it != hex.end(); )
    {
        int hi = hex_value(*it++);
        int lo = hex_value(*it++);
        output.push_back(hi << 4 | lo);
    }
    return output;
}

std::string Utils::ASCIItoHEX(string& ascii)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(ascii.length() * 2);
    for (unsigned char c : ascii)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

bool Utils::file_exists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}