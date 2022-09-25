#include "../include/Server.h"
Server::Server(Coin &coin)
{   
    c = &coin;
}

DWORD Server::InitializeServerThread() {
    // create socket
    int listen_sock = get_listen_socket();

    // set reuse
    set_reuse_addr(listen_sock);

    // bind
    listen_socket_bind(listen_sock);

    // listen
    listen_socket_listen(listen_sock);

    // socklen_t addr_size = sizeof(sockaddr_in);
    pollfd listen_pollfd;
    listen_pollfd.fd = listen_sock;
    listen_pollfd.events = POLLIN;
    poll_sets.push_back(listen_pollfd);

    while (1)
    {
        int ret = WSAPoll((pollfd*)&poll_sets[0], (unsigned int)poll_sets.size(), 150);

        if (ret <= 0)
        {
            // timeout is error
            perror("poll error");
            exit(1);
        }

        // check every pool in the set
        vector<pollfd>::iterator it;
        vector<pollfd>::iterator end = poll_sets.end();
        for (it = poll_sets.begin(); it != end; it++)
        {
            if (it->revents & POLLIN)
            {
                if (it->fd == listen_sock)
                {
                    // new client
                    int client_sock = accept_socket(listen_sock);
                    if (client_sock == -1)
                    {
                        perror("accept failed");
                        continue;
                    }

                    if (poll_sets.size() - 1 < MAX_CLIENT_NUM)
                    {
                        // add to pool set
                        add_client(client_sock);
                    }
                    else
                    {
                        too_many_clients(client_sock);
                    }
                }
                else
                {
                    // client echo message
                    char* message = new char[BUF_SIZE];
                    int read_size = recv(it->fd, message, BUF_SIZE, NULL);

                    if (read_size <= 0)
                    {
                        // client close
                        del_client(it);
                    }
                    else
                    {
                        //Send the message back to client
                        send(it->fd, message, strlen(message) + 1, NULL);
                    }
                    free(message);
                }
            }
            else if (it->revents & POLLERR)
            {
                if (it->fd == listen_sock)
                {
                    // listen socket error
                    perror("listen socket error");
                    exit(1);
                }
                else
                {
                    // client error
                    del_client(it);
                }
            }
        }
    }
}

void Server::add_client(int client_sock)
{
    pollfd client_pollfd;
    client_pollfd.fd = client_sock;
    client_pollfd.events = POLLIN;
    poll_sets.push_back(client_pollfd);
    printf("now_client_num: %u\n", (unsigned int)poll_sets.size() - 1);
}

void Server::del_client(vector<pollfd>::iterator it)
{
    poll_sets.erase(it);
    closesocket(it->fd);
    printf("now_client_num: %u\n", (unsigned int)poll_sets.size() - 1);
}

vector<uint8_t> Server::serializate_block(Block b) {
    /*
    <--ORDER OF DATA-->
    1 - Num of bytes on Block (4 bytes)
    2 - Hash (32 bytes)
    3 - Previous Hash (32 bytes)
    4 - Hash Merkle Root (32 bytes)
    5 - Height (4 bytes)
    6 - Nonce (4 bytes)
    7 - Timestamp (4 bytes)
    8 - Num of transactions (2 bytes)
    9 - Transactions array (variable)
    */

    //CREATE THE ARRAY TO PUT THE SERIALIZATED DATA OF THE OBJECT TRANSACTION
    vector<uint8_t> serializated_block;
    
    //1 - LOAD THE NUMBER OF BYTES
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_block.push_back(uint8_t(b.getNumBytes() >> (UINT32_LENGTH - i - 1)));
    }
    //2 - LOAD THE HASH 
    for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
        serializated_block.push_back(b.getHash().bits[i]);
    }
    // 3 - LOAD THE PREVIOUS HASH 
    for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
        serializated_block.push_back(b.getPrevHash().bits[i]);
    }
    //4 - LOAD THE HASH MERKLE ROOT
    for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
        serializated_block.push_back(b.getHashMerkleRoot().bits[i]);
    }
    //5 - LOAD THE HEIGHT
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_block.push_back(uint8_t(b.getHeight() >> (UINT32_LENGTH - i - 1)));
    }
    //6 - LOAD THE NONCE
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_block.push_back(uint8_t(b.getNonce() >> (UINT32_LENGTH - i - 1)));
    }
    //7 - LOAD THE DATE
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_block.push_back(uint8_t(b.getDate() >> (UINT32_LENGTH - i - 1)));
    }
    //8 - LOAD THE NUMBER OF TRANSACTIONS
    for (uint8_t i = 0; i < UINT16_LENGTH; i++) {
        serializated_block.push_back(uint8_t(b.getNumTX() >> (UINT16_LENGTH - i - 1)));
    }
    //9 - FOR EACH TRANSATION WE SERIALIZATE EACH ONE OF THEM AND THEN WE PUT THEM TO THE SERIALIZATED BLOCK
    vector<Transaction> transactions = b.getTX();
    for (uint16_t i = 0; i < b.getNumTX(); i++) {
        vector<uint8_t> data_transaction = serializate_transaction(transactions[i]);
        for (uint32_t j = 0; j < data_transaction.size(); j++) {
            serializated_block.push_back(data_transaction[j]);
        }
    }

    //RETURN THE SERIALIZATED DATA OF THE BLOCK
    return serializated_block;
}

vector<uint8_t> Server::serializate_transaction(Transaction t)
 {
    /*
    <--ORDER OF DATA-->
    1 - Num of bytes on Transaction (4 bytes)
    2 - Hash From (32 bytes)
    3 - Hash To (32 bytes)
    4 - Amount (4 bytes)
    5 - Timestamp (4 bytes)
    6 - Signature Length (4 bytes)
    7 - Signature (variable)
    */

    //CREATE THE ARRAY TO PUT THE SERIALIZATED DATA OF THE OBJECT TRANSACTION
    vector<uint8_t> serializated_transaction;

    //1 - LOAD THE NUMBER OF BYTES
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_transaction.push_back(uint8_t(t.getNumBytes() >> (UINT32_LENGTH - i - 1)));
    }
    //2 - LOAD THE HASH FROM
    for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
        serializated_transaction.push_back(t.getTxFrom().bits[i]);
    }
    //3 - LOAD THE HASH TO
    for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
        serializated_transaction.push_back(t.getTxTo().bits[i]);
    }
    //4 - LOAD THE AMOUNT OF TX
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_transaction.push_back(uint8_t(t.getAmount() >> (UINT32_LENGTH - i - 1)));
    }
    //5 - LOAD THE TIME OF THE TRANSACTION
    for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
        serializated_transaction.push_back(uint8_t(t.getTime() >> (UINT32_LENGTH - i - 1)));
    }
    //6 - LOAD THE LENGTH OF THE SIGNATURE
    for (uint8_t i = 0; i < t.getSignatureLength(); i++) {
        serializated_transaction.push_back(uint8_t(t.getSignatureLength() >> (UINT32_LENGTH - i - 1)));
    }
    //7 - LOAD THE SIGNATURE
    for (uint32_t i = 0; i < t.getSignatureLength(); i++) {
        serializated_transaction.push_back(t.getSignature()[i]);
    }

    //RETURN OUR OBJECT SERIALIZATED
    return serializated_transaction;
}

SOCKADDR_IN Server::deserializate_socket_address() {
    //FALTA
}