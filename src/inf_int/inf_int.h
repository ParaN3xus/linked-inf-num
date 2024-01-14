#ifndef INF_INT_H
#define INF_INT_H

#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"

class inf_int {
private:
    bool sign;
    uint_linked_list digits;

    void unify_zero_sign();
    void normalize();

    static inf_int abs_add(const inf_int& a, const inf_int& b);
    static inf_int abs_sub(const inf_int& a, const inf_int& b);
    static inf_int abs_mul(const inf_int& a, const inf_int& b);
    static inf_int abs_mul(const inf_int& a, const unsigned int& b);
    static bool is_abs_less_than(const inf_int& a, const inf_int& b);

public:
    inf_int();
    inf_int(const inf_int& other);
    inf_int(const std::string& num);

    bool is_positive() const;
    std::string to_string(const bool& comma) const;

    void lshift32(const unsigned int& a);

    static inf_int add(const inf_int& a, const inf_int& b);
    static inf_int sub(const inf_int& a, const inf_int& b);
    static inf_int mul(const inf_int& a, const inf_int& b);
    static bool is_equal(const inf_int& a, const inf_int& b);
    static bool is_less_than(const inf_int& a, const inf_int& b);

    friend std::ostream& operator<<(std::ostream& output, const inf_int& num);

    ~inf_int();
};

inf_int operator-(const inf_int& a, const inf_int& b);
inf_int operator+(const inf_int& a, const inf_int& b);
inf_int operator*(const inf_int& a, const inf_int& b);


bool operator==(const inf_int& a, const inf_int& b);
bool operator<(const inf_int& a, const inf_int& b);
bool operator>(const inf_int& a, const inf_int& b);
bool operator<=(const inf_int& a, const inf_int& b);
bool operator>=(const inf_int& a, const inf_int& b);


#endif // INF_INT_H