#ifndef UINT_LINKED_LIST_H
#define UINT_LINKED_LIST_H

class uint_linked_list {
private:
    struct Node {
        unsigned int data;
        Node* next;
    };

    Node* head;

public:
    uint_linked_list();
    uint_linked_list(const uint_linked_list& other);
    ~uint_linked_list();

    void append(unsigned int data);
    void insert(int index, unsigned int data);
    void remove(unsigned int data);
    int length();
    std::string to_bit_string();
    
    unsigned int& operator[](int index);
    uint_linked_list& operator=(const uint_linked_list& other);
};

#endif // UINT_LINKED_LIST_H