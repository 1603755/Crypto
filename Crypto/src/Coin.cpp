#include "../include/Coin.h"



Coin::Coin()
{
	DIFFICULTY = 0;
	MAX_FEE = 10;
	PORT = 10000;
	timeout = 150;
}

//OK. DONE
bool Coin::addCandidateBlock(Block block) {
	if (!block.verifyHash(sha256::toString(block.hash.bits), DIFFICULTY))
		return false;
	if (BlockChain.size() != 0 && block.getHeight() < getLastBlockOnChain().getHeight())
		return false;
	//En caso de que el bloque ya este en la cola de candidatos ya no hace falta que lo añadamos nuevamente
	queue<Block> copy_candidates = candidates;
	for (int i = 0; i < candidates.size(); i++) {
		Block front = copy_candidates.front();
		if (sha256::toString(front.getHash().bits) == sha256::toString(block.getHash().bits))
			return true;
		copy_candidates.pop();
	}
	//Ahora tenemos que añadirlo a la BlockChain
	candidates.push(block);

	//Ahora vamos a añadir el bloque a la BlockChain
	bool done;
	do {
		done = true;
		for (int i = 0; i < candidates.size(); i++) {
			Block front = candidates.front();
			bool accepted = addBlockToChain(front);
			if (accepted) {
				candidates.pop();
			}
			else {
				done = false;
			}

		}
	} while (!done);

	return true;
}

//OK, UNDONE
bool Coin::addBlockToChain(Block block) {
	Block last = getLastBlockOnChain();
	if (BlockChain.size() == 0) {
		if (sha256::toString(block.getHash().bits)== "" and sha256::toString(block.getPrevHash().bits) != "" and block.getHeight() != NULL and sha256::toString(block.getHashMerkleRoot().bits) == "")
			return false;
	}
	else
		if (sha256::toString(block.getPrevHash().bits) == "")
			return false;
	if (sha256::toString(last.getHash().bits) != sha256::toString(block.getPrevHash().bits))
		return false;
	if (last.getHeight() + 1 != block.getHeight())
		return false;
	if (sha256::toString(block.getHashMerkleRoot().bits) == "")
		return false;
	if (!block.verifyHash(sha256::toString(block.getHashMerkleRoot().bits), DIFFICULTY));
	return false;
	//Faltaria validar la comision del bloque y luego las transacciones
	BlockChain.push_back(block);

	saveBlockchainAsJSON();

	return true;
}

//OK. DONE
void Coin::saveBlockchainAsJSON() {
	string parseBC = "[";
	for (int i = 0; i < BlockChain.size(); i++) {
		parseBC += "{";
		parseBC += "\"Height\":" + BlockChain[i].getHeight() + ',';
		parseBC += "\"Hash\":" + sha256::toString(BlockChain[i].getHash().bits) + ',';
		parseBC += "\"Nonce\":" + BlockChain[i].getNonce() + ',';
		parseBC += "\"Hash Merkle Root\":" + sha256::toString(BlockChain[i].getHashMerkleRoot().bits) + ',';
		parseBC += "\"Previous Hash\":" + sha256::toString(BlockChain[i].getPrevHash().bits) + ',';
		parseBC += "\"Date\":" + BlockChain[i].getDate() + ',';
		parseBC += "\"Transactions\":";
		for (int j = 0; j < BlockChain[i].getTX().size(); j++) {
			parseBC += "{";
			parseBC += "\"From\":" + sha256::toString(BlockChain[i].getTX()[j].getTxFrom().bits) + ',';
			parseBC += "\"To\":" + sha256::toString(BlockChain[i].getTX()[j].getTxTo().bits) + ',';
			parseBC += "\"Amount\":" + BlockChain[i].getTX()[j].getAmount() + ',';
			parseBC += "\"Date\":" + BlockChain[i].getTX()[j].getTime();
			if (j != BlockChain[i].getTX().size() - 1)
				parseBC += "},";
			else
				parseBC += "}";
		}
		if (i != BlockChain.size() - 1)
			parseBC += "},";
		else
			parseBC += "}";
	}
	parseBC += "]";
	char* content = const_cast<char*>(parseBC.c_str());

	errno_t error;
	FILE* file;
	error = fopen_s(&file, "blockchain.json", "w, css=UTF-8");
	if (error == 0)
		printf("The file 'blockchain.json' was opened\n");
	else
		printf("The file 'blockchain.json' was not opened\n");
	
	fputs(content, file);
	error = fclose(file);
	if (error == 0)
		printf("The file 'blockchain.json' was closed\n");
	
	else
		printf("The file 'blockchain.json' was not closed\n");
	
}

//OK. DONE
void Coin::readjustDifficulty() {
	int seconds = 60 * 60 * 24 * 7;
	int nblocks = 10 * 6 * 24 * 7;
	int real_blocks = 0;
	int i = BlockChain.size() - 1;
	while (BlockChain[i].getDate() > BlockChain[BlockChain.size() - 1].getDate() - seconds) {
		i--;
		real_blocks++;
	}

	/*Reajustamos la dificultad teniendo en cuenta que se deben generar 1 bloque cada 10 min, si el ratio de bloques minado real
	entre el esperado es superior a uno esto implica que se debe aumentar la dificultad ya que se han minado mas bloques de los
	que debian, lo mismo pasa en viceversa*/
	double ratio = (real_blocks) / (nblocks);
	if (ratio > 1)
		DIFFICULTY++;
	else if (ratio < 1)
		DIFFICULTY--;
}

bool Coin::saveBlockChain() {

	//Save missing blocks into local files
	for (uint64_t i = 0; i < BlockChain.size(); i++) {
		if (!Utils::file_exists(BLOCKCHAIN_DATA_PATH + to_string(i) + ".dat")) {
			ofstream file;
			file.open(BLOCKCHAIN_DATA_PATH + to_string(i) + ".dat"); //,ios::app) with append
			file.write((char*)&BlockChain[i], sizeof(BlockChain[i]));
			file.close();
		}
	}	
	return true;
}

bool Coin::loadBlockChain() {
	return true;
}

void Coin::readjustReward() {
	
}

Block Coin::getBlock(uint64_t num) {
	if (num > BlockChain.size() - 1) {
		Block b;
		return b;
	}
	else {
		return BlockChain[num];
	}
}
