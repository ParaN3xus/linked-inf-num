#include <math.h>
#include "inf_int.h"
#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"
#include <ostream>

inf_int::inf_int() : sign(false) {
}

inf_int::inf_int(const inf_int& other) {
    sign = other.sign;
    digits = other.digits;
}

inf_int::inf_int(const std::string& s) {
    std::string num = s;
    if (num[0] == '-') {
        sign = true;
        num.erase(num.begin());
    }
    else {
        sign = false;
    }

    digits.append(0);

    int cnt = 0;
    std::string binary = inf_num_str2bin_str(num);
    for (int i = binary.size() - 1; i >= 0; --i) {
        digits[0] += (binary[i] - '0') << cnt;
        cnt++;
        if (cnt == INF_INT_DIGIT_SIZE) {
            digits.insert(0, 0);
            cnt = 0;
        }
    }

    digits.remove_leading_zeros();
}

inf_int::~inf_int() {
}


bool inf_int::is_positive() const {
    return !sign;
}

std::string inf_int::to_string(const bool& comma = false) const {
    std::string binstr = digits.to_bit_string();

    std::string res = "0";
    for (int i = 0; i < binstr.length(); ++i) {
        res = binstr_mut2(res);
        if (binstr[i] == '1') {
            res = binstr_add1(res);
        }
    }

    if (comma) {
        res = add_commas(res);
    }

    if(sign) {
        res = "-" + res;
    }

    return res;
}


inf_int inf_int::abs_add(const inf_int& l, const inf_int& r) {
    inf_int a = l, b = r;
    inf_int tmp;

    int len_a = a.digits.length();
    int len_b = b.digits.length();

    bool carry = false;
    bool last_carry = false;
    unsigned long long t;
    for (int i = 0; i < std::min(len_a, len_b); ++i) {
        carry = is_add_overflow(a.digits[len_a - i - 1], b.digits[len_b - i - 1], last_carry);
        t = (unsigned long long)a.digits[len_a - i - 1] + b.digits[len_b - i - 1] + last_carry;
        if (carry) {
            t -= UINT_MAX + 1;
        }
        last_carry = carry;
        tmp.digits.insert(0, t);
    }

    inf_int* x, * y;
    int len_x, len_y;

    if (len_a < len_b) {
        x = &b;
        y = &a;
        len_x = len_b;
        len_y = len_a;
    }
    else {
        x = &a;
        y = &b;
        len_x = len_a;
        len_y = len_b;
    }

    for (int i = len_y; i < len_x; ++i) {
        carry = is_add_overflow(x->digits[len_x - i - 1], last_carry);
        t = (unsigned long long)x->digits[len_x - i - 1] + last_carry;
        if (carry) {
            t -= UINT_MAX + 1;
        }

        last_carry = carry;
        tmp.digits.insert(0, t);
    }

    if (carry != 0) {
        tmp.digits.insert(0, carry);
    }

    tmp.digits.remove_leading_zeros();

    return tmp;
}

// +a - +b
inf_int inf_int::abs_sub(const inf_int& l, const inf_int& r) {
    inf_int a = l, b = r;

    if (is_abs_less_than(a, b)) {
        inf_int tmp = abs_sub(b, a);
        tmp.sign = true;
        return tmp;
    }

    int len_a = a.digits.length();
    int len_b = b.digits.length();

    inf_int res;
    unsigned int tmp;

    bool borrow = false;
    bool last_borrow = false;
    for (int i = 0; i < std::min(len_a, len_b); ++i) {
        borrow = is_sub_overflow(a.digits[len_a - i - 1], b.digits[len_b - i - 1], last_borrow);
        if (borrow) {
            tmp = UINT_MAX - b.digits[len_b - i - 1] - last_borrow;
            tmp += a.digits[len_a - i - 1] + 1;
        }
        else {
            tmp = a.digits[len_a - i - 1] - b.digits[len_b - i - 1] - last_borrow;
        }
        last_borrow = borrow;

        res.digits.insert(0, tmp);
    }

    for (int i = len_b; i < len_a; ++i) {
        borrow = is_sub_overflow(a.digits[len_a - i - 1], last_borrow);
        if (borrow) {
            tmp = UINT_MAX;
        }
        else {
            tmp = a.digits[len_a - i - 1] - last_borrow;
        }
        last_borrow = borrow;

        res.digits.insert(0, tmp);
    }

    res.digits.remove_leading_zeros();

    return res;
}

void inf_int::lshift32(const unsigned int& a) {
    int len = digits.length();
    for (int i = 0; i < a; ++i) {
        digits.insert(len, 0);
        ++len;
    }
}

inf_int inf_int::abs_mul(const inf_int& x, const unsigned int& b) {
    inf_int res, a = x;
    unsigned long long tmp = 0;
    unsigned int carry = 0;

    // a * b          + carry
    // ((2^32 - 1)^2) + (((2^32 - 1)^2)/2^32) < 2^64

    for (int i = a.digits.length() - 1; i >= 0; --i) {
        tmp = (unsigned long long)a.digits[i] * (unsigned long long)b + (unsigned long long)carry;
        carry = (unsigned int)(tmp >> INF_INT_DIGIT_SIZE);  // /
        unsigned int t = tmp & (unsigned long long)UINT_MAX;
        res.digits.insert(0, t);               // %
    }

    if (carry != 0) {
        res.digits.insert(0, carry);
    }

    res.digits.remove_leading_zeros();

    return res;
}

inf_int inf_int::abs_mul(const inf_int& x, const inf_int& y) {
    inf_int res, a = x, b = y;
    inf_int tmp;

    int len_b = b.digits.length();

    for (int i = len_b - 1; i >= 0; --i) {
        tmp = abs_mul(a, b.digits[i]);
        tmp.lshift32(len_b - i - 1);
        res = abs_add(res, tmp);
    }

    res.digits.remove_leading_zeros();

    return res;
}

inf_int inf_int::add(const inf_int& a, const inf_int& b) {
    // + +
    if (!a.sign && !b.sign) {
        return abs_add(a, b);
    }

    // + -
    if (!a.sign && b.sign) {
        return abs_sub(a, b);
    }

    // - +
    if (a.sign && !b.sign) {
        return abs_sub(b, a);
    }

    // - -
    inf_int tmp;
    tmp = abs_add(a, b);

    tmp.sign = true;
    return tmp;
}

inf_int inf_int::sub(const inf_int& a, const inf_int& b) {
    // + +
    if (!a.sign && !b.sign) {
        return abs_sub(a, b);
    }

    // + -
    if (!a.sign && b.sign) {
        return abs_add(a, b);
    }

    inf_int tmp;
    // - +
    if (a.sign && !b.sign) {
        tmp = abs_add(a, b);
        tmp.sign = true;
        return tmp;
    }

    // - -
    tmp = abs_sub(a, b);
    tmp.sign = !tmp.sign;
    return tmp;
}

inf_int inf_int::mul(const inf_int& a, const inf_int& b) {
    // + + or - -
    if (a.sign == b.sign) {
        return abs_mul(a, b);
    }

    // + - or - +
    inf_int tmp;
    tmp = abs_mul(a, b);
    tmp.sign = true;
    return tmp;
}


bool inf_int::is_equal(const inf_int& a, const inf_int& b) {
    if (a.sign != b.sign)
        return false;

    return uint_linked_list::is_equal(a.digits, b.digits);
}

bool inf_int::is_abs_less_than(const inf_int& a, const inf_int& b) {
    return uint_linked_list::is_bitval_less_than(a.digits, b.digits);
}

bool inf_int::is_less_than(const inf_int& a, const inf_int& b) {
    // + -
    if (!a.sign && b.sign)
        return false;

    // - +
    if (a.sign && !b.sign)
        return true;

    // - -
    if (a.sign && b.sign)
        return !is_abs_less_than(a, b);

    // + +
    return is_abs_less_than(a, b);
}


inf_int operator+(const inf_int& a, const inf_int& b) {
    return inf_int::add(a, b);
}

inf_int operator-(const inf_int& a, const inf_int& b) {
    return inf_int::sub(a, b);
}

inf_int operator*(const inf_int& a, const inf_int& b) {
    return inf_int::mul(a, b);
}

bool operator==(const inf_int& a, const inf_int& b) {
    return inf_int::is_equal(a, b);
}

bool operator<(const inf_int& a, const inf_int& b) {
    return inf_int::is_less_than(a, b);
}

bool operator<=(const inf_int& a, const inf_int& b) {
    return a < b || a == b;
}

bool operator>(const inf_int& a, const inf_int& b) {
    return !(a <= b);
}

bool operator>=(const inf_int& a, const inf_int& b) {
    return !(a < b);
}

std::ostream& operator<<(std::ostream& output, const inf_int& num) {
    output << num.to_string(false);
    return output;
}