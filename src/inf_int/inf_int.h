#ifndef INF_INT_H
#define INF_INT_H

#include <string>
#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"

class inf_int {
private:
    bool sign;
    uint_linked_list *digits;

    static inf_int abs_add(inf_int a, inf_int b);
    static inf_int abs_sub(inf_int a, inf_int b);

public:
    inf_int();
    inf_int(std::string num);

    bool is_positive();
    static inf_int add(inf_int a, inf_int b);
    static inf_int sub(inf_int a, inf_int b);

    ~inf_int();
};


#endif // INF_INT_H