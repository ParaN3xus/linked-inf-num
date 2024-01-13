#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include<string>

std::string inf_num_str2bin_str(std::string num);

bool is_add_overflow(unsigned int a, unsigned int b, bool carry);
bool is_add_overflow(unsigned int a, unsigned int b);
bool is_add_overflow(unsigned int a, bool carry);

std::string binstr_mut2(const std::string& num);
std::string binstr_add1(const std::string& num);

const int INF_INT_DIGIT_SIZE = sizeof(int) * 8;
#define UINT_MAX INF_INT_DIGIT_SIZE == 32 ? UINT32_MAX : UINT64_MAX

#endif // COMMON_UTILS_H