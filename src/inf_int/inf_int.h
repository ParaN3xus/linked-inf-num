#ifndef INF_INT_H
#define INF_INT_H

#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"

class inf_int {
private:
    bool sign;
    uint_linked_list digits;

    static inf_int abs_add(inf_int a, inf_int b);
    static inf_int abs_sub(inf_int a, inf_int b);
    static bool is_abs_less_than(inf_int a, inf_int b);

public:
    inf_int();
    inf_int(const inf_int& other);
    inf_int(std::string num);

    bool is_positive();
    std::string to_string(bool comma);

    static inf_int add(inf_int a, inf_int b);
    static inf_int sub(inf_int a, inf_int b);
    static bool is_equal(inf_int a, inf_int b);
    static bool is_less_than(inf_int a, inf_int b);

    ~inf_int();
};

inf_int operator-(inf_int a, inf_int b);
inf_int operator+(inf_int a, inf_int b);


bool operator==(inf_int a, inf_int b);
bool operator<(inf_int a, inf_int b);
bool operator>(inf_int a, inf_int b);
bool operator<=(inf_int a, inf_int b);
bool operator>=(inf_int a, inf_int b);

#endif // INF_INT_H