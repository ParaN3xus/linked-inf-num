#include <stdexcept>
#include <string>
#include <bitset>
#include "uint_linked_list.h"
#include "../utils/common_utils.h"

uint_linked_list::uint_linked_list() : head(nullptr) {}

uint_linked_list::uint_linked_list(const uint_linked_list& other) {
    head = nullptr;

    Node* cur = other.head;
    while (cur != nullptr) {
        append(cur->data);
        cur = cur->next;
    }
}

uint_linked_list::~uint_linked_list() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}

void uint_linked_list::append(const unsigned int& data) {
    Node* newNode = new Node{ data, nullptr };
    if (head == nullptr) {
        head = newNode;
    }
    else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// insert before the index, new elem will become new elem on index
void uint_linked_list::insert(const int& index, const unsigned int& data) {
    if (index < 0) return;
    Node* newNode = new Node{ data, nullptr };
    if (index == 0) {
        newNode->next = head;
        head = newNode;
    }
    else {
        Node* current = head;
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
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    else {
        Node* prev = nullptr;
        Node* current = head;
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
    Node* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

std::string uint_linked_list::to_bit_string() const {
    std::string res = "";

    Node* current = head;
    while (current != nullptr) {
        res = res + std::bitset<INF_INT_DIGIT_SIZE>(current->data).to_string();
        current = current->next;
    }
    return res;
}

bool uint_linked_list::is_equal(const uint_linked_list& a, const uint_linked_list& b) {
    Node* cur_a = a.head;
    Node* cur_b = b.head;
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

    Node* cur_a = a.head;
    Node* cur_b = b.head;

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

    Node* tmp = head;
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
