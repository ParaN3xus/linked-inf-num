#include "common_utils.h"

std::string inf_int_str2bin_str(const std::string& n) {
    std::string num = n;
    std::string binary;
    while (num != "0") {
        binary = ((num.back() - '0') % 2 ? "1" : "0") + binary;

        // div
        int carry = 0;
        for (int i = 0; i < num.size(); ++i) {
            int current = num[i] - '0' + carry * 10;
            num[i] = current / 2 + '0';
            carry = current % 2;
        }

        // remove leading zeros
        while (num.size() > 1 && num[0] == '0') {
            num.erase(num.begin());
        }
    }
    return binary;
}

std::string inf_frac_str2bin_str(const std::string& frac, const int& bits) {
    std::size_t zeros;
    std::string num = frac;
    std::string res = "";
    int times = 0;

    while (num != "") {
        if (times >= bits)
            goto INF_FRAC_STR2BIN_STR_END;

        int carry = 0;
        std::string temp = "";
        for (int i = num.size() - 1; i >= 0; --i) {
            int digit = (num[i] - '0') * 2 + carry;
            carry = digit / 10;
            digit %= 10;
            temp = char(digit + '0') + temp;
        }
        res = res + char(carry + '0');

        size_t lastNonZero = temp.find_last_not_of('0');
        if (lastNonZero != std::string::npos) {
            num = temp.substr(0, lastNonZero + 1);
        }
        else {
            num = "";
        }
        times++;
    }

    zeros = std::max(static_cast<std::size_t>(0), bits - res.length());
    res = res + std::string(zeros, '0');

INF_FRAC_STR2BIN_STR_END:
    return res;
}

bool is_add_overflow(unsigned int a, bool carry) {
    if (carry && a == UINT_MAX) {
        return true;
    }
    return false;
}

bool is_add_overflow(unsigned int a, unsigned int b) {
    if (a > UINT_MAX - b) {
        return true;
    }
    return false;
}

bool is_add_overflow(unsigned int a, unsigned int b, bool carry) {
    if (is_add_overflow(a, b)) {
        return true;
    }

    if (carry && a + b == UINT_MAX) {
        return true;
    }

    return false;
}

bool is_sub_overflow(unsigned int a, bool borrow) {
    if (borrow && a == 0) {
        return true;
    }
    return false;
}

bool is_sub_overflow(unsigned int a, unsigned int b, bool borrow) {
    if (b > a) {
        return true;
    }

    // b = a 
    if (b == a) {
        return borrow;
    }

    // b < a
    return false;
}

bool is_vaild_int(const std::string& num) {
    if (num.empty()) {
        return false;
    }

    size_t start = (num[0] == '-') ? 1 : 0;

    for (size_t i = start; i < num.length(); ++i) {
        if (!std::isdigit(num[i])) {
            return false;
        }
    }

    return true;
}

bool is_vaild_decimal(const std::string& num) {
    if (num.empty()) {
        return false;
    }

    size_t start = (num[0] == '-') ? 1 : 0;

    bool foundpoint = false;

    for (size_t i = start; i < num.length(); ++i) {
        if (std::isdigit(num[i])) {
            continue;
        } else if (num[i] == '.' && !foundpoint) {
            foundpoint = true;
        } else {
            return false;
        }
    }

    return true;
}


std::string decstr_mut2(const std::string& num) {
    std::string result;
    int carry = 0;
    for (int i = num.length() - 1; i >= 0; --i) {
        int digit = (num[i] - '0') * 2 + carry;
        carry = digit / 10;
        digit %= 10;
        result = std::to_string(digit) + result;
    }
    if (carry > 0) {
        result = std::to_string(carry) + result;
    }
    return result;
}

std::string decstr_add1_i(const std::string& num) {
    std::string result = num;
    int carry = 1;
    for (int i = num.length() - 1; i >= 0 && carry > 0; --i) {
        int digit = (num[i] - '0') + carry;
        carry = digit / 10;
        digit %= 10;
        result[i] = digit + '0';
    }
    if (carry > 0) {
        result = "1" + result;
    }
    return result;
}

std::string decstr_div2(const std::string& decimal) {
    std::string result;
    int remainder = 0;
    bool isFraction = false;
    for (char c : decimal) {
        if (c == '.') {
            result += '.';
            isFraction = true;
        }
        else {
            int digit = c - '0';
            int value = remainder * 10 + digit;
            result += std::to_string(value / 2);
            remainder = value % 2;
        }
    }
    if (remainder != 0 && !isFraction) {
        result += ".5";
    }
    else {
        while (remainder != 0 && isFraction) {
            remainder *= 10;
            result += std::to_string(remainder / 2);
            remainder %= 2;
        }
    }

    return result;
}

std::string decstr_add1(const std::string& decimal) {
    std::string num = decimal;
    for (int i = num.size() - 1; i >= 0; --i) {
        if (num[i] == '.') {
            continue;
        }
        int digit = num[i] - '0';
        if (++digit < 10) {
            num[i] = digit + '0';
            return num;
        }
        num[i] = '0';
    }
    return '1' + num;
}


std::string add_commas(const std::string& input) {
    size_t point = input.find('.');

    std::string ints = input.substr(0, point);
    std::string decs = input.substr(point + 1);

    if (ints[0] == '-') {
        ints = '-' + ints.substr(1);
    }

    int count = 0;
    for (int i = ints.size() - 1; i >= 0; --i) {
        if (ints[i] == ',') {
            continue;
        }

        count++;

        if (count % 3 == 0 && i > 0) {
            ints.insert(i, 1, ',');
        }
    }

    return ints + ((point == std::string::npos) ? ("") : ("." + decs));
}

unsigned int leftshift_1fixed(unsigned int num, unsigned int bits) {
    return (num << bits) | ((1 << bits) - 1);
}