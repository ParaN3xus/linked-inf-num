# linked_inf_num

`linked_inf_num` is a C++ library designed for **high-precision arithmetic operations**. It utilizes a linked list-based approach to handle numbers with arbitrary precision, making it suitable for applications that require accurate calculations beyond the limitations of standard data types.

## Features

- **Arbitrary Precision:** Perform arithmetic operations on numbers with precision limited only by available memory.
- **Basic Arithmetic Operations:** Support for addition, subtraction, multiplication, and division operations with high precision.
- **32-bit Unsigned Integer Storage:** The library uses 32-bit unsigned integers as the basic storage unit, effectively representing numbers in a base-2^32 system. This approach allows for efficient storage and accelerated calculations using circuit-level adders, providing improved performance compared to decimal simulations.

    > **Note:** This storage method unavoidably leads to the inability to precisely represent certain decimal floating-point numbers. However, it guarantees accurate decimal precision for the fractional part, which is 32 * lg(2) * n, where n is the number of unsigned integers used for the decimal fraction part.

- **Linked List Implementation:** The library uses a linked list structure to dynamically allocate memory for storing digits. 

    > **Note:** The use of linked lists may **NOT** be an optimal choice in terms of memory and time efficiency. However, it is a mandatory requirement for this library as my C++ course assignment. If performance is a concern, consider implementing `uint_linked_list` using `vector<unsigned int>`.

## Usage

### Installation

Clone the repository and modify `CMakeLists.txt`, for example:

```cmake
aux_source_directory(. SRCS)
add_library (linked_inf_num ${SRCS})

add_subdirectory(uint_linked_list)
add_subdirectory(inf_int)
add_subdirectory(inf_float)
add_subdirectory(utils)

target_link_libraries(linked_inf_num inf_int inf_float)
```

Include necessary headers in your source file.

```cpp
#include "linked_inf_num/src/linked_inf_num.h"
```

### Example

```cpp
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
```

In this example, the parameter `3` in `inf_float()` and `inf_float::div()` means number of unsigned int of the decimal part.

## TODO

- [ ] Replace `uint_linked_list` with `uint_vec`.
- [ ] Mathematical functions.
- [ ] More operators.
- [ ] Calculations between `inf_num` and `int` and `float`, `inf_int` and `inf_float`.
- [ ] `inf_real` class.
- [ ] Detailed documents.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This library is licensed under the GPLv3 License.
