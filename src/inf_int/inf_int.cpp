#include <math.h>
#include "inf_int.h"
#include "../utils/common_utils.h"
#include "../uint_linked_list/uint_linked_list.h"

inf_int::inf_int() {
}

inf_int::inf_int(const inf_int& other) {
    sign = other.sign;
    digits = other.digits;
}

inf_int::inf_int(std::string num) {
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
}

inf_int::~inf_int() {
}


bool inf_int::is_positive() {
    return !sign;
}

std::string inf_int::to_string(bool comma = false) {
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

    return res;
}


inf_int inf_int::abs_add(inf_int a, inf_int b) {
    inf_int tmp;

    int len_a = a.digits.length();
    int len_b = b.digits.length();

    bool carry = false;
    for (int i = 0; i < std::min(len_a, len_b); ++i) {
        carry = is_add_overflow(a.digits[len_a - i - 1], b.digits[len_b - i - 1], carry);
        tmp.digits.insert(0, a.digits[len_a - i - 1] + b.digits[len_b - i - 1] + carry);
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
        tmp.digits.insert(0, x->digits[len_x - i - 1] + carry);
        carry = is_add_overflow(x->digits[len_x - i - 1], carry);
    }

    return tmp;
}

// +a - +b
inf_int inf_int::abs_sub(inf_int a, inf_int b) {
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

    return res;
}

inf_int inf_int::add(inf_int a, inf_int b) {
    inf_int tmp;

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
    tmp = a + b;

    tmp.sign = true;
    return tmp;
}

inf_int inf_int::sub(inf_int a, inf_int b) {
    inf_int tmp;

    // + +
    if (!a.sign && !b.sign) {
        return abs_sub(a, b);
    }

    // + -
    if (!a.sign && b.sign) {
        return abs_add(a, b);
    }

    // - +
    if (a.sign && !b.sign) {
        tmp = abs_add(a, b);
        tmp.sign = true;
        return tmp;
    }

    // - -
    tmp = abs_sub(a, b);
    tmp.sign = true;
    return tmp;
}

bool inf_int::is_equal(inf_int a, inf_int b) {
    if (a.sign != b.sign)
        return false;

    return uint_linked_list::is_equal(a.digits, b.digits);
}

bool inf_int::is_abs_less_than(inf_int a, inf_int b) {
    return uint_linked_list::is_bitval_less_than(a.digits, b.digits);
}

bool inf_int::is_less_than(inf_int a, inf_int b) {
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


inf_int operator+(inf_int a, inf_int b) {
    return inf_int::add(a, b);
}

inf_int operator-(inf_int a, inf_int b) {
    return inf_int::sub(a, b);
}

bool operator==(inf_int a, inf_int b) {
    return inf_int::is_equal(a, b);
}

bool operator<(inf_int a, inf_int b) {
    return inf_int::is_less_than(a, b);
}

bool operator<=(inf_int a, inf_int b) {
    return a < b || a == b;
}

bool operator>(inf_int a, inf_int b) {
    return !(a <= b);
}

bool operator>=(inf_int a, inf_int b) {
    return !(a < b);
}