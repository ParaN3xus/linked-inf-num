#include "common_utils.h"

std::string inf_num_str2bin_str(std::string num) {
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

std::string binstr_mut2(const std::string& num) {
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

std::string binstr_add1(const std::string& num) {
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
