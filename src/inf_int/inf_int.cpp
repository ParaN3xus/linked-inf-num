#include "inf_int.h"
#include "../uint_linked_list/uint_linked_list.cpp"

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
    digits.~uint_linked_list();
}

