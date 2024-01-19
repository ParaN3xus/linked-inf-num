#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include<string>

std::string inf_int_str2bin_str(const std::string& num);
std::string inf_frac_str2bin_str(const std::string& num, const int& bits);

bool is_add_overflow(unsigned int a, unsigned int b, bool carry);
bool is_add_overflow(unsigned int a, unsigned int b);
bool is_add_overflow(unsigned int a, bool carry);

bool is_sub_overflow(unsigned int a, unsigned int b, bool borrow);
bool is_sub_overflow(unsigned int a, bool borrow);

std::string binstr_mut2(const std::string& num);
std::string binstr_add1(const std::string& num);
std::string decstr_div2(const std::string& decimal);
std::string decstr_add1(const std::string& decimal);

std::string add_commas(const std::string& input);

unsigned int leftshift_1fixed(unsigned int a, unsigned int b);

const int INF_INT_DIGIT_SIZE = 32;
#define UINT_MAX UINT32_MAX

#endif // COMMON_UTILS_H