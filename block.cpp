#include "head.h"

block::block(uint32_t nIndexIn){
    nIndex = nIndexIn;
}

block::block(uint32_t nIndexIn, vector<transaction> vChainCurrentTransactions):nIndex(nIndexIn), vBlockTransactions(vChainCurrentTransactions){
    nNonce = -1;
    tTime = time(nullptr);
}


void block::setTransactions(vector<transaction> chainCurrentTransactions){
    vBlockTransactions = chainCurrentTransactions;
}

uint32_t block::mineBlock(uint32_t nDifficulty, uint32_t maxTries){
    uint32_t tries = 0;
    char cstr[nDifficulty+1];
    // cout << "Bloko mininimo foro pradzia" << endl;
    for(uint32_t i = 0; i < nDifficulty; i++){
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';
    string str(cstr);
    string sHash = "";
    do{
        nNonce++;
        sHash = "";
        sHash = calculateHash();
        tries++;
        if(tries == maxTries){
            break;
        }
    }
    while(sHash.substr(0, nDifficulty)!=str);

    return tries;
}

inline string block::calculateHash() const {
    stringstream ss;
    ss << nIndex << tTime << nNonce << sPrevHash << sMerkleRootHash;
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        ss << vBlockTransactions[i].stringify();
    }
    return sha256(ss.str());
}

string block::getContent() const {
    stringstream ss;
    ss << "Indeksas: " << nIndex << ", laikas: " << tTime << ", nonsas: " << nNonce << ", prevHash: " << sPrevHash << ", MerkleRootHash: " << sMerkleRootHash << ", ";
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        ss << vBlockTransactions[i].stringify();
    }
    return ss.str();
}

void block::merkleTree(){
    vector<vector<string> > hashes;
    vector<string> tempVector;

    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        tempVector.push_back(sha256(vBlockTransactions[i].stringify()));
    }
    
    hashes.push_back(tempVector);

    uint32_t i = 0;
    while( i < hashes.size()){
        // cout << "Hashes.size - " << hashes.size() << endl;
        // cout << "Hashes[" << i <<"].size - " << hashes[i].size() << endl;
        vector<string> tempVec;
        string hash1 = "";
        string hash2 = "";
        for(uint32_t j = 0; j < hashes[i].size(); j++){
            if(j%2 == 1){
                string hashSum = hashes[i][j-1] + hashes[i][j];
                tempVec.push_back(sha256(hashSum));
            }
            if(j == hashes[i].size()-1 && j%2 != 1){
                tempVec.push_back(sha256(hashes[i][j]));
            }
        }
        // for(uint32_t k = 0; k < tempVec.size(); k++){
            // cout << "Blokas " << i+1 << " -> " << k << " - " << tempVec[k] << endl;
        // }
        hashes.push_back(tempVec);
        if(tempVec.size() == 1){
            break;
        }
        i++;
    }

    sMerkleRootHash = hashes.back()[0];

}

void block::execTransactions(userPool &users){
    uint32_t corrTrans = 0;
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        vBlockTransactions[i].executeTransaction(users);
        if(vBlockTransactions[i].nStatus == 2){
            vBlockTransactions.erase(vBlockTransactions.begin() + i);
            i--;
            // cout << "Transaction index - " << i << " - removed" << endl;
        }
        else if(vBlockTransactions[i].nStatus == 3){
            vBlockTransactions.erase(vBlockTransactions.begin() + i);
            i--;
            corrTrans++;
        }
        // cout << "Transaction index - " << i << endl;
    }
    if(corrTrans > 0){
        cout << "Number of corrupt transactions: " << corrTrans << endl;
    }
}