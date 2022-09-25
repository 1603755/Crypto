#include "../include/Client.h"

Client::Client(Coin &coin) {
	*c = coin;
}

DWORD Client::InitializeClientThread() {
	while (true) {
		for (uint32_t i = 0; i < peers.size(); i++) {
			int status = connect_to_peer(peers[i]);
			if (status == PEER_RESPONSE_ERROR) {
				peers.erase(peers.begin() + i);
				continue;
			}
		}
	}
	
}

void Client::DiscoverPeers()
{
	//WE CREATE THE SOCKET
	int sockfd = socket(AF_INET, SOCK_STREAM, NULL);
	if (sockfd == INVALID_SOCKET) {
		std::cout << "Cannot get the socket correctly";
	}
	SOCKADDR_IN peer_addr;
	peer_addr.sin_family = AF_INET;
	peer_addr.sin_port = c->PORT;
	peer_addr.sin_addr.s_addr = INADDR_ANY;
	//CONNECT TO PEER
	int connect_result = connect(sockfd, (sockaddr*)&peer_addr, sizeof(SOCKADDR_IN));

	if (connect_result > 0)
	{
		std::cout << "Connection refused";
	}
}

int Client::connect_to_peer(SOCKADDR_IN peer)
{
	//WE CREATE THE SOCKET
	int sockfd = socket(AF_INET, SOCK_STREAM, NULL);
	if (sockfd == INVALID_SOCKET) {
		std::cout << "Cannot get the socket correctly";
	}
	//CONNECT TO PEER
	int connect_result = connect(sockfd, (sockaddr*)&peer, sizeof(SOCKADDR_IN));
	if (connect_result > 0)
	{
		std::cout << "Connection refused";
	}
	//FOR EACH BLOCK WE WILL REQUESTING THE DATA OF EACH ONE OF THEM, WE ARE GONNA LOKKING FOR NEW BLOCK ON CHAIN
	uint32_t BLOCK_REQUESTED = c->getLastBlockOnChain().getHeight() + 1;
	uint8_t type;
	do
	{
		//PREPARE THE MESSAGE REUEST
		char request[RAW_MESSAGE_SIZE];
		request[0] = BLOCK_REQUEST;
		for (uint8_t i = 1; i <= UINT32_LENGTH; i++) {
			request[i] = request[i] | uint8_t(BLOCK_REQUESTED >> (UINT32_LENGTH - i - 1)*SHIFT_BYTE);
		}
		//HERE WE SEND OUR MESSAGE REQUEST
		size_t ctl_send = send(sockfd, (char*)request, RAW_MESSAGE_SIZE, NULL);
		
		queue<char> response;
		size_t message_length = recv(sockfd, (char*)&response, RAW_MESSAGE_SIZE + MAX_BLOCK_SIZE, NULL);
		//READ DATA FROM BUFFER
		type = response.front();
		response.pop();
		//LOAD BLOCK NUMBER
		uint32_t nBlock;
		for (uint8_t i = UINT32_LENGTH  - 1; i >= 0 ; i--) {
			nBlock = nBlock |response.front();
			response.pop();
			nBlock = nBlock << SHIFT_BYTE* i;
		}
		//THe BLOCK RESPONSED IS NOT THE SAME WE ASKED, SO STOP CONNECTING TO THIS PEER
		if (nBlock != BLOCK_REQUESTED) {
			return PEER_RESPONSE_ERROR;
		}

		queue<uint8_t> block_data;
		uint8_t bytesBlock = response.front();
		block_data.push(bytesBlock);
		response.pop();
		for (size_t i = 0; i < bytesBlock; i++) {
			block_data.push(response.front());
			response.pop();
		}
		Block BlockRequested;
		BlockRequested = deserializate_block(block_data);
		if (!c->addCandidateBlock(BlockRequested)) {
			return PEER_RESPONSE_ERROR;
		}
		BLOCK_REQUESTED++;
		//IF THE PEER RESPONSE IS "NA" THEN BLOCKCHAIN DOESN'T HAVE ANY MORE BLOCKS
	} while (type != BLOCK_RESPONSE_NA);
	//WE GET ALL LAST BLOCK FROM PEERS, SO WE'RE DONE
	return PEER_RESPONSE_OK;
}

Block Client::deserializate_block(queue<uint8_t> block_data) 
{
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

	//LOAD NUM BYTES
	uint32_t num_bytes = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = block_data.front();
		num_bytes = (num_bytes << 8) | byte;
		block_data.pop();
	}
	//LOAD THE HASH BLOCK
	uint256_t Hash;
	for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
		Hash.bits[i] = block_data.front();
		block_data.pop();
	}
	//LOAD THE PREVIOUS HASH BLOCK
	uint256_t PrevHash;
	for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
		PrevHash.bits[i] = block_data.front();
		block_data.pop();
	}
	//LOAD THE HASH MERKLE ROOT
	uint256_t HashMerkleRoot;
	for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
		HashMerkleRoot.bits[i] = block_data.front();
		block_data.pop();
	}
	//LOAD THE HEIGHT
	uint32_t Height = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = block_data.front();
		Height = (Height << 8) | byte;
		block_data.pop();
	}
	//LOAD THE NONCE
	uint32_t Nonce = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = block_data.front();
		Nonce = (Nonce << 8) | byte;
		block_data.pop();
	}
	//LOAD THE TIMESTAMP
	uint32_t Timestamp = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = block_data.front();
		Timestamp = (Timestamp << 8) | byte;
		block_data.pop();
	}
	//LOAD THE NUM OF TRANSACTIONS
	uint32_t nTransactions = 0;
	for (uint8_t i = 0; i < UINT16_LENGTH; i++) {
		uint8_t byte = block_data.front();
		nTransactions = (nTransactions << 8) | byte;
		block_data.pop();
	}
	vector<Transaction> transactions;
	for (uint32_t i = 0; i < nTransactions; i++) {
		uint32_t bytes_tx = block_data.front();
		queue<uint8_t> raw_data_transaction;
		for (uint32_t j = 0; j < bytes_tx; j++) {
			raw_data_transaction.push(block_data.front());
			block_data.pop();
		}
		Transaction tx = deserializate_transaction(raw_data_transaction);
		transactions.push_back(tx);
	}

	Block b(num_bytes, Hash, PrevHash, HashMerkleRoot, Height, Nonce, Timestamp, nTransactions, transactions);
	return b;
}

CTransaction Client::deserializate_transaction(queue<uint8_t> transaction_data) 
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

	//LOAD NUM BYTES
	uint32_t num_bytes = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = transaction_data.front();
		num_bytes = (num_bytes << 8) | byte;
		transaction_data.pop();
	}
	//LOAD THE HASH FROM
	uint256_t HashFrom;
	for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
		HashFrom.bits[i] = transaction_data.front();
		transaction_data.pop();
	}
	//LOAD THE HASH TO
	uint256_t HashTo;
	for (uint8_t i = 0; i < UINT256_LENGTH; i++) {
		HashTo.bits[i] = transaction_data.front();
		transaction_data.pop();
	}
	//LOAD THE AMOUNT
	uint32_t amount = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = transaction_data.front();
		amount = (amount << 8) | byte;
		transaction_data.pop();
	}
	//LOAD THE TIMESTAMP
	uint32_t timestamp = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = transaction_data.front();
		timestamp = (timestamp << 8) | byte;
		transaction_data.pop();
	}
	//LOAD TEH SIGNATURE LENGTH
	uint32_t signature_length = 0;
	for (uint8_t i = 0; i < UINT32_LENGTH; i++) {
		uint8_t byte = transaction_data.front();
		signature_length = (signature_length << 8) | byte;
		transaction_data.pop();
	}
	//LOAD THE SIGNATURE
	vector<uint8_t> sign;
	for (uint32_t i = 0; i < signature_length; i++) {
		sign.push_back(transaction_data.front());
		transaction_data.pop();
	}

	//RETURN THE TRANSACTION WITH THE DATA ALREADY DESERIALIZATED
	Transaction tx(num_bytes, HashFrom, HashTo, amount, timestamp, signature_length, sign);
	return tx;
}

queue<uint8_t> Client::serializate_socket_address(SOCKADDR_IN socket_addr)
{ //FALTA
	queue<uint8_t> raw_address;
	raw_address.push(socket_addr.sin_family);
	raw_address.push((socket_addr.sin_port >> SHIFT_BYTE) & 0x00ff);
	raw_address.push(socket_addr.sin_port & 0x00ff);	
	string ip = inet_ntoa(socket_addr.sin_addr);
	for (uint8_t i = 0; i < ip.length(); i++) {
		raw_address.push(ip[i]);
	}
	return raw_address;
}