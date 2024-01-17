#ifndef INF_FLOAT_H
#define INF_FLOAT_H

#include "../inf_int/inf_int.h"

class inf_float {
private:
    inf_int mantissa;
    int exponent;

    void unify_zero_sign();
    void normalize();

    static inf_float abs_add(const inf_float& a, const inf_float& b);
    static inf_float abs_sub(const inf_float& a, const inf_float& b);
    static inf_float abs_mul(const inf_float& a, const inf_float& b);
    static bool is_abs_less_than(const inf_float& a, const inf_float& b);

public:
    inf_float();
    inf_float(const inf_float& other);
    inf_float(const std::string& num, const int& precisionZ);

    std::string to_string(const bool& comma) const;

    static inf_float add(const inf_float& a, const inf_float& b);
    static inf_float sub(const inf_float& a, const inf_float& b);
    static inf_float mul(const inf_float& a, const inf_float& b);
    static bool is_equal(const inf_float& a, const inf_float& b);
    static bool is_less_than(const inf_float& a, const inf_float& b);

    friend std::ostream& operator<<(std::ostream& output, const inf_float& num);

    ~inf_float();
};

#endif //INF_FLOAT_H