#include "common_utils.h"

std::string infNumStr2BinStr(std::string num) {
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

const int INF_INT_DIGIT_SIZE = sizeof(int) * 8;