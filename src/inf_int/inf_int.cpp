#include "inf_int.h"
#include "../utils/common_utils.cpp"
#include "../uint_linked_list/uint_linked_list.cpp"
#include <math.h>
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


inf_int inf_int::abs_add(inf_int a, inf_int b) {
    inf_int* tmp = new inf_int;

    bool carry = false;
    for (int i = 0; i < std::min(a.digits.length(), b.digits.length()); ++i) {
        tmp->digits.insert(0, a.digits[a.digits.length() - i - 1] + b.digits[b.digits.length() - i - 1] + carry);
        carry = is_add_overflow(a.digits[a.digits.length() - i - 1], b.digits[b.digits.length() - i - 1], carry);
    }

    if (a.digits.length() < b.digits.length()) {
        std::swap(a, b);
    }

    for (int i = b.digits.length(); i < a.digits.length(); ++i) {
        tmp->digits.insert(0, a.digits[a.digits.length() - i - 1] + carry);
        carry = is_add_overflow(a.digits[a.digits.length() - i - 1], carry);
    }

    return *tmp;
}

inf_int inf_int::abs_sub(inf_int a, inf_int b) {
    inf_int tmp;
    // to do

    return tmp;
}

inf_int inf_int::add(inf_int a, inf_int b) {
    inf_int tmp;

    // + +
    if (a.is_positive() && b.is_positive()) {
        return abs_add(a, b);
    }

    // + -
    if (a.is_positive() && !b.is_positive()) {
        return abs_sub(a, b);
    }

    // - +
    if (!a.is_positive() && b.is_positive()) {
        return abs_sub(b, a);
    }

    // - -
    if (!a.is_positive() && !b.is_positive()) {
        tmp = a + b;

        tmp.sign = true;
        return tmp;
    }

    return tmp;
}

inf_int inf_int::sub(inf_int a, inf_int b) {
    inf_int tmp;

    // + +
    if (a.is_positive() && b.is_positive()) {
        return abs_sub(a, b);
    }

    // + -
    if (a.is_positive() && !b.is_positive()) {
        return abs_add(a, b);
    }

    // - +
    if (!a.is_positive() && b.is_positive()) {
        tmp = abs_add(a, b);
        tmp.sign = true;
        return tmp;
    }

    // - -
    if (!a.is_positive() && !b.is_positive()) {
        tmp = abs_sub(a, b);
        tmp.sign = true;
        return tmp;
    }

    return tmp;
}


inf_int operator+(inf_int a, inf_int b) {
    return inf_int::add(a, b);
}

inf_int operator-(inf_int a, inf_int b) {
    return inf_int::sub(a, b);
}
