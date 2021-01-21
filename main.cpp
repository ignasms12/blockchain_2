#include "head.h"


userPool::userPool(){
    for(uint32_t i = 0; i < 1000; i++){
        string name = "user_" + to_string(i);
        user temp_user(name);
        string userAddress = temp_user.getAddress();
        users.push_back(temp_user);
        uuidMap[userAddress] = i;
    }
    cout << "Mapo dydis - " << uuidMap.size() << endl;
}

user* userPool::getUserByUuid(string uuid){
    uint32_t index = uuidMap[uuid];
    vector<user>::iterator it = users.begin() + index;
    return &(*it);
}

void generateTransaction(vector<transaction> &transactions, userPool users){
    uint32_t senderIndex = rand() % 1000;
    uint32_t receiverIndex = rand() % 1000;
    uint32_t senderBalance = users.getUserByIndex(senderIndex).getBalance();
    uint32_t sum = rand() % senderBalance;

    if(senderIndex != receiverIndex || senderBalance >= sum){
        transaction temp_trans(users.getUserByIndex(senderIndex).getAddress(), users.getUserByIndex(receiverIndex).getAddress(), sum);
        transactions.push_back(temp_trans);
    }
    else if(senderIndex == receiverIndex){
        generateTransaction(transactions, users);
    }
}


int main(){
    blockchain bChain = blockchain();


    // Creating users
    userPool users;

    // bChain.addBlock(users); // kodel ????
    vector<transaction> transactionPool;


    for(uint32_t i = 0; i < 1000; i++){
        generateTransaction(transactionPool, users);
    }

    cout << "transactionPool.size at start - " << transactionPool.size() << endl;

    uint32_t lastTransSize = -1;

    while(transactionPool.size() > 0){
        uint32_t i = 0;
        if(transactionPool.size() <= 100){
            lastTransSize = transactionPool.size();
        }
        while(i < 100){
            uint32_t randIndex = rand() % transactionPool.size();

            bChain.vChainCurrentTransactions.push_back(transactionPool[randIndex]);
            transactionPool.erase(transactionPool.begin() + randIndex);
            if(i == lastTransSize){
                break;
            }
            i++;
        }o
        cout << "Mining block..." << endl;
        bChain.addBlock(users);

    }




    vector<block> current_chain = bChain.getChain();
    // for(uint32_t i = 0; i < current_chain.size(); i++){
    //     cout << endl;
    //     cout << "transactions of " << i << " block: " << current_chain[i].getContent() << endl;
    //     cout << endl;
    // }

    return 0;
}