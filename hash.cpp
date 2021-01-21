#include "head.h"

string to_hex(unsigned char s){
    stringstream ss;
    ss << hex << (uint32_t) s;
    return ss.str();
}   

string sha256(string line){    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, line.c_str(), line.length());
    SHA256_Final(hash, &sha256);

    string output = "";    
    for(uint32_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        output += to_hex(hash[i]);
    }
    return output;
}