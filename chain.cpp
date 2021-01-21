#include "head.h"

blockchain::blockchain(){
    nDifficulty = 1;
    version = "0.2";
    nodeAddress = uuidGen();
    vChain.push_back(block(0, vChainCurrentTransactions));
    vChain.back().mineBlock(nDifficulty, 100000000);
}

void blockchain::addBlock(userPool &users){
    cout << "Chaino dydis - " << vChain.size() << endl;
    
    newTransaction("0",nodeAddress,1);
    block bNew(vChain.size(), vChainCurrentTransactions);
    bNew.execTransactions(users);
    bNew.merkleTree();

    vChainCurrentTransactions.clear();

    bNew.sPrevHash = getLastBlock().calculateHash();
    vChain.push_back(bNew);

    cout << "Block mined " << bNew.calculateHash() << endl;
    cout << endl;
}

block blockchain::getLastBlock() const{
    return vChain.back();
}

uint32_t blockchain::newTransaction(string sSender,string sReceiver,uint32_t nSum){
    vChainCurrentTransactions.push_back(transaction(sSender,sReceiver,nSum));
    return vChain.back().nIndex;
}