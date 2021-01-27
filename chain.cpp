#include "head.h"

blockchain::blockchain(){
    nDifficulty = 1;
    version = "0.2";
    nodeAddress = uuidGen();
    vChain.push_back(block(0, vChainCurrentTransactions));
    vChain.back().mineBlock(nDifficulty, 100000000);
}

uint32_t mine(uint32_t maxTries, vector<block> blocks, uint32_t nDifficulty){
    uint32_t blockIndex = 0;
    vector<int>::iterator it;
    vector<int> usedBlocks;
    uint32_t randIndex = 0;
    for(int i = 0; i < 5; i++){
        do{
            randIndex = rand() % 5;
            it = find(usedBlocks.begin(), usedBlocks.end(), randIndex);
            if(usedBlocks.size() == 0){
                break;
            }
        }
        while(it != usedBlocks.end());
        usedBlocks.push_back(randIndex);
        uint32_t tries = 0;
        tries = blocks[i].mineBlock(nDifficulty, maxTries);
        if(tries <= maxTries){
            blockIndex = i;
            break;
        }
        else{
            if(i == 4){
                maxTries += 100000;
                mine(maxTries, blocks, nDifficulty);
            }   
        }
    }
    return blockIndex;
}


uint32_t blockchain::addBlock(userPool &users, vector<vector<transaction>> transactions){

    vector<block> blocks;
    uint32_t maxTries = 100000;

    for(int i = 0; i < 5; i++){
        transaction miningTrans("0", nodeAddress, 1);
        transactions[i].push_back(miningTrans);
        bc::hash_list hashes;
        for(uint32_t j = 0; j < transactions[i].size(); j++){
            uint32_t n = transactions[i][j].getId().length();
            char c[65];
            strcpy(c, transactions[i][j].getId().c_str());
            hashes.push_back(bc::hash_literal(c));
        }

        block bNew(vChain.size(), transactions[i]);
        bNew.merkleTree(hashes);
        bNew.sPrevHash = getLastBlock().calculateHash();
        blocks.push_back(bNew);
    }

    uint32_t blockIndex = mine(maxTries, blocks, nDifficulty);

    // cout << "Dabartinio bloko transakciju kiekis - " << blocks[blockIndex].getTransSize() << endl;
    blocks[blockIndex].execTransactions(users);
    vChain.push_back(blocks[blockIndex]);

    cout << "Block mined " << blocks[blockIndex].calculateHash() << endl;
    cout << endl;

    blocks.clear();

    return blockIndex;
}

block blockchain::getLastBlock() const{
    return vChain.back();
}

uint32_t blockchain::newTransaction(string sSender,string sReceiver,uint32_t nSum){
    vChainCurrentTransactions.push_back(transaction(sSender,sReceiver,nSum));
    return vChain.back().nIndex;
}