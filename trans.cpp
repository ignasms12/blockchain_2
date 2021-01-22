#include "head.h"


transaction::transaction(string sSenderIn, string sReceiverIn, uint32_t nSumIn):sReceiver(sReceiverIn),sSender(sSenderIn), nSum(nSumIn){
    nStatus = 0;
    stringstream ss;
    ss << "sender: " << sSender << " , receiver: " << sReceiver << " , amount: " << nSum;
    sTransactionId = sha256(ss.str());
}

string transaction::stringify() const{
    stringstream ss;
    ss << "sender: " << sSender << " , receiver: " << sReceiver << " , amount: " << nSum;
    return ss.str();
}

void transaction::executeTransaction(userPool &users){
    user* sender = users.getUserByUuid(sSender);
    user* receiver = users.getUserByUuid(sReceiver);
    if(sha256(this->stringify()) != sTransactionId){
        nStatus = 3;
    }
    else{
        if(sender->getBalance() < nSum){
            nStatus = 2;
        }
        else{
            nStatus = 1;
            sender->updateBalance(nSum * (-1));
            receiver->updateBalance(nSum);
        }
    }
}