#ifndef kQueue_h
#define kQueue_h

#include "kMemoryAllocator.h"

template<typename T>
class kQueue {

    struct Element {
        T* data;
        Element* next;
    };

    Element* head;
    Element* tail;

public:

    Element* getIterator() {
        return head;
    }

    kQueue() {
        head = nullptr;
        tail = nullptr;
    }

    kQueue(const kQueue&) = delete;

    bool isEmpty() const {
        return (head == nullptr);
    }

    void pushData(T* data) {
        auto new_element = (Element*) kMemoryAllocator::instance().kmem_alloc(sizeof(Element));
        new_element->data = data;
        new_element->next = nullptr;

        if (tail) {
            tail->next = new_element;
            tail = new_element;
        } else {
            head = new_element;
            tail = new_element;
        }
    }

    T* popData() {
        if (!head) {
            return nullptr;
        }

        Element* return_element = head;
        head = head->next;

        if (!head) {
            tail = nullptr;
        }

        T* return_data = return_element->data;
        kMemoryAllocator::instance().mem_free(return_element);
        return return_data;
    }

    bool find(T* data) {
        for (Element* current = head; current != nullptr; current = current->next) {
            if (current->data == data)
                return true;
        }

        return false;
    }

    T* removeElement(T* data) {
        Element* previous = nullptr;

        for (Element* current = head; current != nullptr; current = current->next) {
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

                T* return_data = current->data;
                kMemoryAllocator::instance().mem_free(current);
                return return_data;
            }

            previous = current;
        }

        return nullptr;
    }
};

#endif