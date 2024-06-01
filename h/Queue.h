#ifndef Queue_h
#define Queue_h

#include "kMemoryAllocator.h"

template<typename T>
class Queue {
protected:

    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;
    int _size;

public:

    Queue() : head(nullptr), tail(nullptr), _size(0) {}

    Queue(const Queue&) = delete;

    Node* getIterator() {
        return head;
    }

    bool isEmpty() const {
        return (head == nullptr);
    }

    int size() const {
        return _size;
    }

    void push(const T& data) {
        auto new_element = (Node*) kMemoryAllocator::instance().kmem_alloc(sizeof(Node));
        new_element->data = data;
        new_element->next = nullptr;

        if (tail) {
            tail->next = new_element;
            tail = new_element;
        } else {
            head = new_element;
            tail = new_element;
        }
        _size += 1;
    }

    T pop() {
        Node* return_element = head;
        head = head->next;

        if (!head)
            tail = nullptr;

        T return_data = return_element->data;
        kMemoryAllocator::instance().mem_free(return_element);
        return return_data;
    }

    T& front() {
        return head->data;
    }

    T& back() {
        return tail->data;
    }

    T& get(int index) {
        Node* iterator = head;
        for (int i = 0; i < index; i++) {
            if (iterator == nullptr)
                return nullptr;
            else
                iterator = iterator->next;
        }
        return iterator->data;
    }

    void remove(const T& data) {
        Node* previous = nullptr;

        for (Node* current = head; current != nullptr; current = current->next) {
            if (current->data == data) {
                if (current == head) {
                    head = current->next;
                    if (!head)
                        tail = nullptr;
                } else if (current == tail) {
                    tail = previous;
                    tail->next = nullptr;
                } else {
                    previous->next = current->next;
                }

                kMemoryAllocator::instance().mem_free(current);
            }

            previous = current;
        }

    }
};

#endif