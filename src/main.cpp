#include "linked_inf_num.h"
#include <chrono>
#include <iostream>

const int PRECISION = 2;
const int ITER = 100000;
const int OUTPUT_PRECISION = 30;

int main() {
    linked_inf_num::inf_float res("3", PRECISION);

    linked_inf_num::inf_float dividend("4", PRECISION);

    linked_inf_num::inf_float divisor_a("2", PRECISION);
    linked_inf_num::inf_float divisor_b("3", PRECISION);
    linked_inf_num::inf_float divisor_c("4", PRECISION);

    linked_inf_num::inf_float two("2", PRECISION);


    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITER; i++) {
        if (i % 2 == 0) {
            res = res + dividend / (divisor_a * divisor_b * divisor_c);
        }
        else {
            res = res - dividend / (divisor_a * divisor_b * divisor_c);
        }

        divisor_a = divisor_a + two;
        divisor_b = divisor_b + two;
        divisor_c = divisor_c + two;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << res.to_string(false, OUTPUT_PRECISION, linked_inf_num::ROUND_TO_NEAREST) << std::endl;

    std::cout << "Time taken: " << diff.count() << "s" << std::endl;


    return 0;
}
