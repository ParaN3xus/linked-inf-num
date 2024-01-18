#include <stdexcept>
#include <string>
#include <bitset>
#include "uint_linked_list.h"
#include "../utils/common_utils.h"

uint_linked_list::uint_linked_list() : head(nullptr) {}

uint_linked_list::uint_linked_list(const uint_linked_list& other) {
    head = nullptr;

    node* cur = other.head;
    while (cur != nullptr) {
        append(cur->data);
        cur = cur->next;
    }
}

uint_linked_list::~uint_linked_list() {
    while (head != nullptr) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}

void uint_linked_list::append(const unsigned int& data) {
    node* newNode = new node{ data, nullptr };
    if (head == nullptr) {
        head = newNode;
    }
    else {
        node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// insert before the index, new elem will become new elem on index
void uint_linked_list::insert(const int& index, const unsigned int& data) {
    if (index < 0) return;
    node* newNode = new node{ data, nullptr };
    if (index == 0) {
        newNode->next = head;
        head = newNode;
    }
    else {
        node* current = head;
        for (int i = 0; i < index - 1 && current != nullptr; i++) {
            current = current->next;
        }
        if (current != nullptr) {
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

void uint_linked_list::remove(const int& index) {
    if (index < 0 || index >= length()) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
    else {
        node* prev = nullptr;
        node* current = head;
        int i = 0;

        while (current != nullptr && i < index) {
            prev = current;
            current = current->next;
            i++;
        }

        if (current != nullptr) {
            prev->next = current->next;
            delete current;
        }
    }
}

void uint_linked_list::remove_leading_zeros() {
    while (head != nullptr && head->data == 0) {
        remove(0);
    }
}

bool uint_linked_list::is_zero() const {
    return head == nullptr;
}

int uint_linked_list::length() const {
    int count = 0;
    node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

int uint_linked_list::get_first_one_pos() const {
    int pos = 0;
    node* current = head;
    while (current != nullptr) {
        if (current->data != 0) {
            for (int i = 31; i >= 0; --i) {
                if (current->data & (1 << i)) {
                    return pos + 31 - i;
                }
            }
        }
        pos += 32;
        current = current->next;
    }
    return -1;
}


std::string uint_linked_list::to_bit_string() const {
    std::string res = "";

    node* current = head;
    while (current != nullptr) {
        res = res + std::bitset<INF_INT_DIGIT_SIZE>(current->data).to_string();
        current = current->next;
    }
    return res;
}

bool uint_linked_list::is_equal(const uint_linked_list& a, const uint_linked_list& b) {
    node* cur_a = a.head;
    node* cur_b = b.head;
    while (cur_a != nullptr && cur_b != nullptr) {
        if (cur_a->data != cur_b->data) {
            return false;
        }
        cur_a = cur_a->next;
        cur_b = cur_b->next;
    }
    return cur_a == nullptr && cur_b == nullptr;
}

bool uint_linked_list::is_bitval_less_than(const uint_linked_list& a, const uint_linked_list& b) {
    if (a.length() != b.length()) {
        return a.length() < b.length();
    }

    node* cur_a = a.head;
    node* cur_b = b.head;

    while (cur_a != nullptr && cur_b != nullptr) {
        if (cur_a->data < cur_b->data) {
            return true;
        }
        else if (cur_a->data > cur_b->data) {
            return false;
        }

        cur_a = cur_a->next;
        cur_b = cur_b->next;
    }

    return false;
}


unsigned int& uint_linked_list::operator[](const int& index) {
    if (current != nullptr && index == current_index + 1) {
        current_index++;
        current = current->next;
        return current->data;
    }

    node* tmp = head;
    for (int i = 0; i < index && tmp != nullptr; i++) {
        tmp = tmp->next;
    }
    if (tmp != nullptr) {
        current = tmp;
        current_index = index;

        return tmp->data;
    }
    else {
        throw std::out_of_range("Index out of range");
    }
}

uint_linked_list& uint_linked_list::operator=(const uint_linked_list& other) {
    if (this != &other) {
        uint_linked_list* temp = new uint_linked_list(other);
        head = temp->head;
    }

    return *this;
}
