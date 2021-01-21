#include "head.h"

user::user(string sUsernameIn){
    sUsername = sUsernameIn;
    sUserAddress = uuidGen();
    srand(time(NULL));
    nBalance = rand() % 1000000 + 100;
}

void user::updateBalance(uint32_t nBalanceIn){
    if(nBalanceIn > 0){
        nBalance = nBalance + nBalanceIn;
    }
    else{
        nBalance = nBalance - nBalanceIn;
    }
    // cout << "nBalanceIn: " << nBalanceIn << endl;
    // cout << "nBalance: " << nBalance << endl;
}