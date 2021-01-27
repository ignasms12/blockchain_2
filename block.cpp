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
    ss << nIndex << tTime << nNonce << sPrevHash << bc::encode_base16(sMerkleRootHash);
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        ss << vBlockTransactions[i].stringify();
    }
    return sha256(ss.str());
}

string block::getContent() const {
    stringstream ss;
    ss << "Indeksas: " << nIndex << ", laikas: " << tTime << ", nonsas: " << nNonce << ", prevHash: " << sPrevHash << ", MerkleRootHash: " << bc::encode_base16(sMerkleRootHash) << ", ";
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        ss << vBlockTransactions[i].stringify();
    }
    return ss.str();
}

void block::merkleTree(bc::hash_list& merkle){

    // Stop if hash list is empty or contains one element
    if (merkle.empty()){
        sMerkleRootHash = bc::null_hash;
        return;
    }
    else if (merkle.size() == 1){
        sMerkleRootHash = merkle[0];
        return;
    }
    // While there is more than 1 hash in the list, keep looping...
    while(merkle.size() > 1){
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0)
            merkle.push_back(merkle.back());
        // List size is now even.
        assert(merkle.size() % 2 == 0);
        // New hash list.
        bc::hash_list new_merkle;
        // Loop through hashes 2 at a time.
        for (auto it = merkle.begin(); it != merkle.end(); it += 2){
            // Join both current hashes together (concatenate).
            bc::data_chunk concat_data(bc::hash_size * 2);
            auto concat = bc::serializer<
                decltype(concat_data.begin())>(concat_data.begin());
            concat.write_hash(*it);
            concat.write_hash(*(it + 1));
            // Hash both of the hashes.
            bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
            // Add this to the new list.
            new_merkle.push_back(new_root);
        }
        // This is the new list.
        merkle = new_merkle;
        // DEBUG output -------------------------------------
        // std::cout << "Current merkle hash list:" << std::endl;
        // for (const auto& hash: merkle)
        //     std::cout << "  " << bc::encode_base16(hash) << std::endl;
        // std::cout << std::endl;
        // --------------------------------------------------
    }
    // Finally we end up with a single item.
    sMerkleRootHash = merkle[0];


    // vector<vector<string> > hashes;
    // vector<string> tempVector;

    // for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
    //     tempVector.push_back(sha256(vBlockTransactions[i].stringify()));
    // }
    
    // hashes.push_back(tempVector);

    // uint32_t i = 0;
    // while( i < hashes.size()){
    //     vector<string> tempVec;
    //     string hash1 = "";
    //     string hash2 = "";
    //     for(uint32_t j = 0; j < hashes[i].size(); j++){
    //         if(j%2 == 1){
    //             string hashSum = hashes[i][j-1] + hashes[i][j];
    //             tempVec.push_back(sha256(hashSum));
    //         }
    //         if(j == hashes[i].size()-1 && j%2 != 1){
    //             tempVec.push_back(sha256(hashes[i][j]));
    //         }
    //     }
    //     // for(uint32_t k = 0; k < tempVec.size(); k++){
    //         // cout << "Blokas " << i+1 << " -> " << k << " - " << tempVec[k] << endl;
    //     // }
    //     hashes.push_back(tempVec);
    //     if(tempVec.size() == 1){
    //         break;
    //     }
    //     i++;
    // }

    // sMerkleRootHash = hashes.back()[0];

}

void block::execTransactions(userPool &users){
    uint32_t corrTrans = 0;
    for(uint32_t i = 0; i < vBlockTransactions.size(); i++){
        uint32_t returnValue = vBlockTransactions[i].executeTransaction(users); // 0 - new, 1 - completed, 2 - failed, 3 - corrupt
        if(returnValue == 2){
            vBlockTransactions.erase(vBlockTransactions.begin() + i);
            i--;
            // cout << "Transaction index - " << i << " - removed" << endl;
        }
        else if(returnValue == 3){
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