#include "../include/Utils.h"
#include "../include/Coin.h"
#include "../ConnectionManager/include/ConnectionManager.h"
int main()
{
    Utils::Keys();
    cout <<"Bitcoin Address: " << Utils::getAddressFromPublicKey(Utils::getPublicKey()) << endl;

    uint256_t a;
    for (int i = 0; i < 32; i++)
        a.bits[i] = 1;
    Transaction tx(a, a, 10, 10);

    Coin c;
    uint256_t data;
    for (int i = 0; i < 32; i++) {
        data.bits[i] = 1;
    }
    Block b;
    b.hash = data;
    b.hashMerkleRoot = data;
    b.height = 0;
    b.nonce = 3000;
    c.BlockChain.push_back(b);
    c.saveBlockChain();
    return 0;
}
