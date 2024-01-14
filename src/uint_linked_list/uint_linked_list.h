#ifndef UINT_LINKED_LIST_H
#define UINT_LINKED_LIST_H

class uint_linked_list {
private:
    struct Node {
        unsigned int data;
        Node* next;
    };

    Node* head;
    Node* current = nullptr;
    int current_index = -1;

public:
    uint_linked_list();
    uint_linked_list(const uint_linked_list& other);
    ~uint_linked_list();

    void append(const unsigned int& data);
    void insert(const int& index, const unsigned int& data);
    void remove(const int& index);
    void remove_leading_zeros();

    bool is_zero() const;
    int length() const;
    std::string to_bit_string() const;

    static bool is_equal(const uint_linked_list& a, const uint_linked_list& b);
    static bool is_bitval_less_than(const uint_linked_list& a, const uint_linked_list& b);

    unsigned int& operator[](const int& index);
    uint_linked_list& operator=(const uint_linked_list& other);
};

#endif // UINT_LINKED_LIST_H