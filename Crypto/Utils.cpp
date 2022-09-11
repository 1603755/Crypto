//https://wiki.openssl.org/index.php/EVP_Asymmetric_Encryption_and_Decryption_of_an_Envelope
#include "Utils.h"

using namespace std;

//will return rublic key and private key, consumer must call EVP_PKEY_free to free after use
pair<EVP_PKEY*, EVP_PKEY*> Utils::GetKeyRSApair()
{
    BIGNUM* bne = BN_new();         //refer to https://www.openssl.org/docs/man1.0.2/man3/bn.html
    BN_set_word(bne, RSA_F4);

    int bits = 2048;
    RSA* r = RSA_new();
    RSA_generate_key_ex(r, bits, bne, NULL);  //here we generate the RSA keys

    //we use a memory BIO to store the keys
    BIO* bp_public = BIO_new(BIO_s_mem()); PEM_write_bio_RSAPublicKey(bp_public, r);
    BIO* bp_private = BIO_new(BIO_s_mem()); PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    size_t pri_len = BIO_pending(bp_private);  //once the data is written to a memory/file BIO , we get the size
    size_t pub_len = BIO_pending(bp_public);
    char* pri_key = (char*)malloc(pri_len + 1);
    char* pub_key = (char*)malloc(pub_len + 1);

    BIO_read(bp_private, pri_key, pri_len);   //now we read the BIO into a buffer
    BIO_read(bp_public, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    //printf("\n%s\n:\n%s\n", pri_key, pub_key);fflush(stdout);  //now we print the keys to stdout (DO NOT PRINT private key in production code, this has to be a secret)

    BIO* pbkeybio = NULL;
    pbkeybio = BIO_new_mem_buf((void*)pub_key, pub_len);  //we create a buffer BIO (this is different from the memory BIO created earlier)
    BIO* prkeybio = NULL;
    prkeybio = BIO_new_mem_buf((void*)pri_key, pri_len);

    RSA* pb_rsa = NULL;
    RSA* p_rsa = NULL;

    pb_rsa = PEM_read_bio_RSAPublicKey(pbkeybio, &pb_rsa, NULL, NULL);  //now we read the BIO to get the RSA key
    p_rsa = PEM_read_bio_RSAPrivateKey(prkeybio, &p_rsa, NULL, NULL);

    EVP_PKEY* evp_pbkey = EVP_PKEY_new();  //we want EVP keys , openssl libraries work best with this type, https://wiki.openssl.org/index.php/EVP
    EVP_PKEY_assign_RSA(evp_pbkey, pb_rsa);

    EVP_PKEY* evp_prkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(evp_prkey, p_rsa);

    //clean up
    free(pri_key); free(pub_key);
    BIO_free_all(bp_public); BIO_free_all(bp_private);
    BIO_free(pbkeybio); BIO_free(prkeybio);
    BN_free(bne);
    RSA_free(r);
  
    return { evp_pbkey,evp_prkey };
}

//Let's encrypt
vector<unsigned char> Utils::envelope_seal(EVP_PKEY** pub_key, unsigned char* plaintext, int plaintext_len,
    unsigned char** encrypted_key, int* encrypted_key_len, unsigned char* iv)
{
    EVP_CIPHER_CTX* ctx;
    int ciphertext_len;
    int len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();

    /* Initialise the envelope seal operation. This operation generates
     * a key for the provided cipher, and then encrypts that key a number
     * of times (one for each public key provided in the pub_key array). In
     * this example the array size is just one. This operation also
     * generates an IV and places it in iv. */
    EVP_SealInit(ctx, EVP_aes_256_cbc(), encrypted_key, encrypted_key_len, iv, pub_key, 1);


    int blocksize = EVP_CIPHER_CTX_block_size(ctx);
    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_SealUpdate can be called multiple times if necessary
     */
    vector<unsigned char> cyphered(plaintext_len + blocksize - 1);
    len = cyphered.size();
    EVP_SealUpdate(ctx, &cyphered[0], &len, plaintext, plaintext_len);  //https://www.openssl.org/docs/man1.1.1/man3/EVP_EncryptInit.html
    //The amount of data written depends on the block alignment of the encrypted data. For most ciphers and modes, the amount of data written can be anything from zero bytes to (inl + cipher_block_size - 1) bytes.

    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    EVP_SealFinal(ctx, &cyphered[0] + len, &len);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    cyphered.resize(ciphertext_len);
    return cyphered;
}

vector<unsigned char> Utils::envelope_open(EVP_PKEY* priv_key, unsigned char* ciphertext, int ciphertext_len, unsigned char* encrypted_key, int encrypted_key_len, unsigned char* iv)
{
    EVP_CIPHER_CTX* ctx;
    int len;
    int plaintext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();

    /* Initialise the decryption operation. The asymmetric private key is
     * provided and priv_key, whilst the encrypted session key is held in
     * encrypted_key */
    EVP_OpenInit(ctx, EVP_aes_256_cbc(), encrypted_key, encrypted_key_len, iv, priv_key);

    vector<unsigned char> plaintext(ciphertext_len);
    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_OpenUpdate can be called multiple times if necessary
     */
    EVP_OpenUpdate(ctx, &plaintext[0], &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    EVP_OpenFinal(ctx, &plaintext[0] + len, &len);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return plaintext;
}

string Utils::GetHex(vector<unsigned char> v)
{
    stringstream ss;
    for (size_t i = 0; i < v.size(); ++i)
    {
        char c1[3] = {};
        sprintf(c1, "%02x", v[i]);
        ss << c1;
    }
    return ss.str();
}

vector<unsigned char> Utils::GetBinary(string s)
{
    vector<unsigned char> b;

    for (size_t i = 0; i < s.size(); i = i + 2)
    {
        unsigned int c, c1;
        char str[2] = { s.c_str()[i],  0 }; sscanf(str, "%02x", &c);
        char str1[2] = { s.c_str()[i + 1],0 }; sscanf(str1, "%02x", &c1);

        b.push_back((c << 4) + c1);
    }
    return b;
}


//save keys

bool Utils::generate_key() {
    int				ret = 0;
    RSA* r = NULL;
    BIGNUM* bne = NULL;
    BIO* bp_public = NULL, * bp_private = NULL;

    int				bits = 2048;
    unsigned long	e = RSA_F4;

    // 1. generate rsa key
    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        goto free_all;
    }

    r = RSA_new();
    ret = RSA_generate_key_ex(r, bits, bne, NULL);
    if (ret != 1) {
        goto free_all;
    }
    // 2. save public key
    bp_public = BIO_new_file("public.pem", "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, r);
    if (ret != 1) {
        goto free_all;
    }

    // 3. save private key
    bp_private = BIO_new_file("private.pem", "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    // 4. free
free_all:

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(r);
    BN_free(bne);

    return (ret == 1);
}