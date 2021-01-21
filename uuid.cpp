#include "head.h"

string uuidGen() {
    uuid_t uuidObj;
    char str[100];
    uuid_generate(uuidObj);
    uuid_unparse(uuidObj, str);
    string _uuid(str);
    return _uuid;
}
