#include <stdexcept>
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
inf_float::inf_float(const std::string& n, const int& precision) {
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
    if (is_negative) {
        this->mantissa.sign = true;
    }

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

    if (mantissa.digits.length() == 0) {
        exponent = 0;
        return;
    }

    int trailing_zeros = mantissa.digits.count_trailing_zeros();

    if (trailing_zeros > 0) {
        exponent += trailing_zeros;
        mantissa.rshift32(trailing_zeros);
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
    else if (exponent < -mantissa.digits.length()) {
        int_part_bin = "";
        frac_part_bin = std::string(-exponent * 32, '0') + all;
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

std::string inf_float::to_string(const bool& comma, const int& dec_place, const rounding_style& rounding_style) const {
    std::string raw_str = to_string(false);
    std::string int_part;
    std::string frac_part;

    int dot_pos = raw_str.find('.');
    if (dot_pos == std::string::npos || dec_place <= 0) {
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str;
    }

    // int_part = raw_str.substr(0, dot_pos);
    // frac_part = raw_str.substr(dot_pos + 1, raw_str.length() - dot_pos - 1);

    if (raw_str.length() - dot_pos - 1 <= dec_place) {
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str + std::string(dec_place - (raw_str.length() - dot_pos - 1), '0');
    }

    switch (rounding_style) {
    case ROUND_TOWARD_ZERO:
        raw_str = raw_str.substr(0, 1 + dot_pos + dec_place);
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str;
        break;

    case ROUND_DOWN:
        if (raw_str[0] == '-') {
            raw_str = '-' + abs_round(raw_str.substr(1), dec_place, ROUND_UP);
        }
        else {
            raw_str = abs_round(raw_str, dec_place, ROUND_DOWN);
        }
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str;
        break;

    case ROUND_UP:
        if (raw_str[0] == '-') {
            raw_str = '-' + abs_round(raw_str.substr(1), dec_place, ROUND_DOWN);
        }
        else {
            raw_str = abs_round(raw_str, dec_place, ROUND_UP);
        }
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str;
        break;

    case ROUND_TO_NEAREST:
        raw_str = abs_round(raw_str, dec_place, ROUND_TO_NEAREST);
        if (comma) {
            raw_str = add_commas(raw_str);
        }
        return raw_str;
        break;

    default:
        throw std::invalid_argument("Invalid rounding style");
    }
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
    ans.mantissa = inf_int::abs_add(x.mantissa, y.mantissa);

    ans.normalize();

    return ans;
}

inf_float inf_float::abs_sub(const inf_float& a, const inf_float& b) {
    inf_float x = a, y = b, ans;
    bool x_exponent_bigger = x.exponent > y.exponent;

    if (x_exponent_bigger) {
        x.mantissa.lshift32(x.exponent - y.exponent);
        ans.exponent = y.exponent;
    }
    else {
        y.mantissa.lshift32(y.exponent - x.exponent);
        ans.exponent = x.exponent;
    }

    ans.mantissa = inf_int::abs_sub(x.mantissa, y.mantissa);

    ans.normalize();

    return ans;
}

std::string inf_float::abs_round(const std::string& num, const int& dec_place, const rounding_style& rounding_style) {
    int dot_pos = num.find('.');
    std::string tmp = num.substr(0, dot_pos + 1 + dec_place);

    bool neg, carry;

    switch (rounding_style) {
    case ROUND_DOWN:
        return tmp;
        break;
    case ROUND_UP:
        return decstr_add1(tmp);
        break;
    case ROUND_TO_NEAREST:
        if (tmp[0] == '-') {
            neg = true;
            tmp = tmp.substr(1);
        }
        else {
            neg = false;
        }

        carry = num[dot_pos + 1 + dec_place] >= '5';

        if (neg) {
            carry = !carry;
        }

        if (carry) {
            tmp = decstr_add1(tmp.substr(0, dot_pos + 1 + dec_place));
        }

        if (neg) {
            tmp = '-' + tmp;
        }

        return tmp;
        break;
    default:
        throw std::invalid_argument("Invalid rounding style");
    }
}


bool inf_float::is_abs_less_than(const inf_float& a, const inf_float& b) {
    inf_float x = a, y = b;
    bool x_exponent_bigger = x.exponent > y.exponent;

    if (x_exponent_bigger) {
        x.mantissa.lshift32(x.exponent - y.exponent);
    }
    else {
        y.mantissa.lshift32(y.exponent - x.exponent);
    }

    return inf_int::is_abs_less_than(x.mantissa, y.mantissa);
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
    inf_float ans;
    ans.exponent = a.exponent + b.exponent;
    ans.mantissa = a.mantissa * b.mantissa;

    ans.normalize();

    return ans;
}

inf_float inf_float::div(const inf_float& a, const inf_float& b) {
    inf_float ans;
    ans.exponent = a.exponent - b.exponent;
    ans.mantissa = a.mantissa / b.mantissa;

    ans.normalize();

    return ans;
}

inf_float inf_float::div(const inf_float& a, const inf_float& b, const int& precision) {
    inf_float ans;
    inf_float x = a, y = b;

    if (precision > y.exponent - x.exponent) {
        x.mantissa.lshift32(x.exponent - y.exponent + precision);
    }
    else {
        y.mantissa.lshift32(-x.exponent + y.exponent - precision);
    }

    ans.exponent = -precision;
    ans.mantissa = x.mantissa / y.mantissa;

    ans.normalize();

    return ans;
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

inf_float operator+(const inf_float& a, const inf_float& b) {
    return inf_float::add(a, b);
}

inf_float operator-(const inf_float& a, const inf_float& b) {
    return inf_float::sub(a, b);
}

inf_float operator*(const inf_float& a, const inf_float& b) {
    return inf_float::mul(a, b);
}

inf_float operator/(const inf_float& a, const inf_float& b) {
    return inf_float::div(a, b);
}

bool operator==(const inf_float& a, const inf_float& b) {
    return inf_float::is_equal(a, b);
}

bool operator<(const inf_float& a, const inf_float& b) {
    return inf_float::is_less_than(a, b);
}

bool operator<=(const inf_float& a, const inf_float& b) {
    return a < b || a == b;
}

bool operator>(const inf_float& a, const inf_float& b) {
    return !(a <= b);
}

bool operator>=(const inf_float& a, const inf_float& b) {
    return !(a < b);
}

std::ostream& operator<<(std::ostream& output, const inf_float& num) {
    output << num.to_string(false);
    return output;
}