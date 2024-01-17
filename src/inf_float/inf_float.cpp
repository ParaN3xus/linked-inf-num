#include "inf_float.h"
#include "../utils/common_utils.h"

inf_float::inf_float() {
    this->mantissa = inf_int();
    this->exponent = 0;
}

inf_float::inf_float(const inf_float& other) {
    this->mantissa = other.mantissa;
    this->exponent = other.exponent;
}

// precision in 43e base n is equivalent to precision in 10 base 32lg2 * n
inf_float::inf_float(const std::string& n, const int& precision = 2) {
    std::string num = n;
    std::string int_part;
    std::string frac_part;
    std::string frac_bin;
    bool is_negative = false;
    int cnt = 0;
    int len = 0;

    if (num[0] == '-') {
        is_negative = true;
        num.erase(0, 1);
    }

    int dot_pos = num.find('.');
    if (dot_pos == std::string::npos) {
        dot_pos = num.length();
    }

    int_part = num.substr(0, dot_pos);
    frac_part = num.substr(dot_pos + 1, num.length() - dot_pos - 1);

    this->mantissa = inf_int(int_part);
    len = this->mantissa.digits.length();

    frac_bin = inf_frac_str2bin_str(frac_part, precision * 32);

    for (int i = 0; i < frac_bin.length(); i += 32) {
        unsigned int tmp = 0;
        for (int j = 0; j < 32; j++) {
            tmp += (frac_bin[i + j] - '0') << (31 - j);
        }

        mantissa.digits.insert(len, tmp);
        len++;
    }

    this->exponent = -frac_bin.length() / 32;
}

inf_float::~inf_float() {
}

void inf_float::unify_zero_sign() {
    mantissa.unify_zero_sign();
}

void inf_float::normalize() {
    mantissa.normalize();

    if (exponent > 0) {
        mantissa.lshift32(exponent);
        exponent = 0;
    }
}

std::string inf_float::to_string(const bool& comma) const {
    std::string all = mantissa.digits.to_bit_string();
    std::string int_part_bin;
    std::string frac_part_bin;
    if (exponent >= 0) {
        int_part_bin = all + std::string(exponent * 32, '0');
        frac_part_bin = "";
    }
    else {
        int_part_bin = all.substr(0, 32 * (mantissa.digits.length() + exponent));
        frac_part_bin = all.substr(32 * (mantissa.digits.length() + exponent), -exponent * 32);
    }

    // int
    std::string int_part = "0";
    for (int i = 0; i < int_part_bin.length(); ++i) {
        int_part = binstr_mut2(int_part);
        if (int_part_bin[i] == '1') {
            int_part = binstr_add1(int_part);
        }
    }

    if (comma) {
        int_part = add_commas(int_part);
    }

    if (mantissa.sign) {
        int_part = "-" + int_part;
    }

    // frac
    std::string frac_part = "0";

    for (int i = frac_part_bin.length() - 1; i >= 0; --i) {
        if (frac_part_bin[i] == '1') {
            frac_part[0] = '1';
        }
        frac_part = decstr_div2(frac_part);
    }

    if (frac_part == "0") {
        return int_part;
    }

    return int_part + frac_part.substr(1, frac_part.length() - 1);
}

inf_float inf_float::abs_add(const inf_float& a, const inf_float& b) {
    inf_float x, y, ans;
    //x: bigger exponent
    if (a.exponent > b.exponent) {
        x = a;
        y = b;
    }
    else {
        x = b;
        y = a;
    }

    x.mantissa.lshift32(x.exponent - y.exponent);
    //x.exponent = y.exponent;

    ans.exponent = y.exponent;
    ans.mantissa = x.mantissa + y.mantissa;

    ans.normalize();

    return ans;
}

inf_float inf_float::abs_sub(const inf_float& a, const inf_float& b) {
    inf_float x, y, ans;
    //x: bigger exponent
    if (a.exponent > b.exponent) {
        x = a;
        y = b;
    }
    else {
        x = b;
        y = a;
    }

    x.mantissa.lshift32(x.exponent - y.exponent);
    //x.exponent = y.exponent;

    ans.exponent = y.exponent;
    ans.mantissa = x.mantissa - y.mantissa;

    ans.normalize();

    return ans;
}

bool inf_float::is_abs_less_than(const inf_float& a, const inf_float& b) {
    inf_float x, y;
    //x: bigger exponent
    if (a.exponent > b.exponent) {
        x = a;
        y = b;
    }
    else {
        x = b;
        y = a;
    }

    x.mantissa.lshift32(x.exponent - y.exponent);
    //x.exponent = y.exponent;

    return x.mantissa < y.mantissa;
}


inf_float inf_float::add(const inf_float& a, const inf_float& b) {
    // + +
    if (!a.mantissa.sign && !b.mantissa.sign) {
        return abs_add(a, b);
    }

    // + -
    if (!a.mantissa.sign && b.mantissa.sign) {
        return abs_sub(a, b);
    }

    // - +
    if (a.mantissa.sign && !b.mantissa.sign) {
        return abs_sub(b, a);
    }

    // - -
    inf_float tmp;
    tmp = abs_add(a, b);

    tmp.mantissa.sign = true;
    tmp.unify_zero_sign();

    return tmp;
}

inf_float inf_float::sub(const inf_float& a, const inf_float& b) {
    // + +
    if (!a.mantissa.sign && !b.mantissa.sign) {
        return abs_sub(a, b);
    }

    // + -
    if (!a.mantissa.sign && b.mantissa.sign) {
        return abs_add(a, b);
    }

    inf_float tmp;
    // - +
    if (a.mantissa.sign && !b.mantissa.sign) {
        tmp = abs_add(a, b);
        tmp.mantissa.sign = true;
        tmp.unify_zero_sign();
        return tmp;
    }

    // - -
    tmp = abs_sub(a, b);
    tmp.mantissa.sign = !tmp.mantissa.sign;
    tmp.unify_zero_sign();
    return tmp;
}

inf_float inf_float::mul(const inf_float& a, const inf_float& b) {
    // + + or - -
    if (a.mantissa.sign == b.mantissa.sign) {
        return abs_mul(a, b);
    }

    // + - or - +
    inf_float tmp;
    tmp = abs_mul(a, b);
    tmp.mantissa.sign = true;
    tmp.unify_zero_sign();
    return tmp;
}

bool inf_float::is_equal(const inf_float& a, const inf_float& b) {
    if (a.mantissa.sign != b.mantissa.sign) {
        return false;
    }

    inf_float x, y, ans;
    //x: bigger exponent
    if (a.exponent > b.exponent) {
        x = a;
        y = b;
    }
    else {
        x = b;
        y = a;
    }

    x.mantissa.lshift32(x.exponent - y.exponent);
    //x.exponent = y.exponent;

    return x.mantissa == y.mantissa;
}

bool inf_float::is_less_than(const inf_float& a, const inf_float& b) {
    // + -
    if (!a.mantissa.sign && b.mantissa.sign)
        return false;

    // - +
    if (a.mantissa.sign && !b.mantissa.sign)
        return true;

    // - -
    if (a.mantissa.sign && b.mantissa.sign)
        return !is_abs_less_than(a, b);

    // + +
    return is_abs_less_than(a, b);
}
