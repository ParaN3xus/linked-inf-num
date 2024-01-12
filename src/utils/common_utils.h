#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include<string>

std::string inf_num_str2bin_str(std::string num);
const int INF_INT_DIGIT_SIZE = sizeof(int) * 8;
#define UINT_MAX INF_INT_DIGIT_SIZE == 32 ? UINT32_MAX : UINT64_MAX

#endif // COMMON_UTILS_H