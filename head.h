#include <iostream>
#include <cstdint>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include "openssl/sha.h"
#include <uuid/uuid.h>

using namespace std;

string sha256(string);
string to_hex(unsigned char);
string uuidGen();

class user{
    public:
        user(string sUsernameIn);
        void updateBalance(uint32_t nBalanceIn);
        inline string getAddress(){return sUserAddress;};
        inline uint32_t getBalance(){return nBalance;};
    private:
        string sUserAddress;
        string sUsername;
        uint32_t nBalance;
};

class userPool{
    public:
        userPool();
        inline user getUserByIndex(uint32_t index){return users[index];};
        user* getUserByUuid(string uuid);
    private:
        vector<user> users;
        map<string, uint32_t> uuidMap;
};

class transaction{
    public:
        transaction(string sSender,string sReceiver,uint32_t nSum);
        string stringify() const;
        uint32_t executeTransaction(userPool &users);
    private:
        string sTransactionId;
        string sSender;
        string sReceiver;
        uint32_t nSum;
};

class block{
    public:
        string sPrevHash;
        block(uint32_t nIndexIn);
        block(uint32_t nIndexIn, vector<transaction> vChainCurrentTransactions);
        string getHash();
        uint32_t mineBlock(uint32_t nDifficulty, uint32_t maxTries);
        void setTransactions(vector<transaction> vChainCurrentTransactions);
        string getContent() const;
        uint32_t nIndex;
        string calculateHash() const;
        void merkleTree();
        void execTransactions(userPool &users);
    private:
        int64_t nNonce;
        vector<transaction> vBlockTransactions;
        time_t tTime;
        string version = "1";
        uint32_t nDifficulty;
        string sMerkleRootHash;
};

class blockchain{
    public:
        blockchain();
        uint32_t addBlock(userPool &users, vector<vector<transaction>> transactions);
        uint32_t newTransaction(string sSender,string sReceiver,uint32_t nSum);
        vector<transaction> vChainCurrentTransactions;
        inline vector<block> getChain() {return vChain;};
    private:
        string version;
        uint32_t nDifficulty;
        string nodeAddress;
        vector<block> vChain;
        block getLastBlock()const;
};