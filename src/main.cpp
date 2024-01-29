#include "linked_inf_num.h"
#include <iostream>


int main() {
    linked_inf_num::inf_int a("-12345678901234567890123456789012345678901234567890"), b("98765432198765432198765432198765432198765432198765");
    linked_inf_num::inf_float c("-1234567890123456789012345.6789012345678901234567890", 3), d("9876543219876543219876543.2198765432198765432198765", 3);

    std::cout << a + b << std::endl << a - b << std::endl << a * b << std::endl << a / b << std::endl;
    std::cout << c + d << std::endl << c - d << std::endl << c * d << std::endl << c / d << std::endl;
    std::cout << (linked_inf_num::inf_float::div(c, d, 3)) << std::endl;

    return 0;
}
