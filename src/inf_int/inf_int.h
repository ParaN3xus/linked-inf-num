#ifndef INF_INT_H
#define INF_INT_H

#include <string>
#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"

class inf_int {
private:
    bool sign;
    uint_linked_list digits;
public:
    inf_int(std::string num);
    ~inf_int();
};


#endif // INF_INT_H