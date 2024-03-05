#include <stdexcept>
#include <string>
#include <bitset>
#include <algorithm>
#include "uint_linked_list.h"
#include "../utils/common_utils.h"

namespace linked_inf_num {
    uint_linked_list::uint_linked_list() : vec() {}

    uint_linked_list::uint_linked_list(const uint_linked_list& other) {
        vec = std::vector<unsigned>(other.vec);
    }

    uint_linked_list::~uint_linked_list() {
        //vec.~vector();
    }

    void uint_linked_list::append(const unsigned int& data) {
        vec.push_back(data);
    }

    // insert before the index, new elem will become new elem on index
    void uint_linked_list::insert(const int& index, const unsigned int& data) {
        if (index < 0) return;
        vec.insert(vec.begin() + index, data);
    }

    void uint_linked_list::remove(const int& index) {
        if (index < 0 || index >= length()) {
            throw std::out_of_range("Index out of range");
        }
        vec.erase(vec.begin() + index);
    }

    void uint_linked_list::remove_leading_zeros() {
        auto first_non_zero = std::find_if(vec.begin(), vec.end(), [](int i) { return i != 0; });

        vec.erase(vec.begin(), first_non_zero);
    }

    bool uint_linked_list::is_zero() const {
        return vec.size() == 0;
    }

    int uint_linked_list::length() const {
        return vec.size();
    }

    int uint_linked_list::get_first_one_pos() const {
        int pos = 0;
        auto current = vec.begin();
        while (pos < length()) {
            if (*current != 0) {
                for (int i = 31; i >= 0; --i) {
                    if (*current & (1 << i)) {
                        return pos + 31 - i;
                    }
                }
            }
            pos += INF_INT_DIGIT_SIZE;
            current++;
        }
        return -1;
    }

    int uint_linked_list::count_trailing_zeros() const {
        int count = 0;
        auto current = vec.end() - 1;
        while (current >= vec.begin()) {
            if (*current != 0) {
                return count;
            }
            else {
                count++;
            }
            current--;
        }
        return count;
    }

    std::string uint_linked_list::to_bit_string() const {
        std::string res = "";

        auto current = vec.begin();
        while (current != vec.end()) {
            res = res + std::bitset<INF_INT_DIGIT_SIZE>(*current).to_string();
            current++;
        }
        return res;
    }

    bool uint_linked_list::is_equal(const uint_linked_list& a, const uint_linked_list& b) {
        auto cur_a = a.vec.begin();
        auto cur_b = b.vec.begin();

        if(a.length() != b.length()) {
            return false;
        }

        while (cur_a < a.vec.end() && cur_b < b.vec.end()) {
            if (*cur_a != *cur_b) {
                return false;
            }
            cur_a++;
            cur_b++;
        }

        return true;
    }

    bool uint_linked_list::is_bitval_less_than(const uint_linked_list& a, const uint_linked_list& b) {
        if (a.length() != b.length()) {
            return a.length() < b.length();
        }

        auto cur_a = a.vec.begin();
        auto cur_b = b.vec.begin();

        while (cur_a < a.vec.end() && cur_b < b.vec.end()) {
            if (*cur_a < *cur_b) {
                return true;
            }
            else if (*cur_a > *cur_b) {
                return false;
            }

            cur_a++;
            cur_b++;
        }

        return false;
    }


    unsigned int& uint_linked_list::operator[](const int& index) {
        if (index < 0 || index >= length()) {
            throw std::out_of_range("Index out of range");
        }

        return vec[index];
    }

    uint_linked_list& uint_linked_list::operator=(const uint_linked_list& other) {
        if (this != &other) {
            vec = std::vector<unsigned>(other.vec);
        }

        return *this;
    }
}
