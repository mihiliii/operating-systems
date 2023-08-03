#ifndef kQueue_h
#define kQueue_h

#include "kMemoryAllocator.h"

template <typename T>
class kQueue{

    struct Element{
        T* data;
        Element* next;
    };

    Element* head;
    Element* tail;

public:

    Element* getIterator(){
        return head;
    }

    kQueue(){
        head = nullptr;
        tail = nullptr;
    }

    kQueue(const kQueue&) = delete;

    bool isEmpty() const{
        return (head == nullptr);
    }

    void pushData(T* data){
        auto newElement = (Element*) kMemoryAllocator::getInstance().kmem_alloc(sizeof(Element));
        newElement->data = data;
        newElement->next = nullptr;

        if (tail){
            tail->next = newElement;
            tail = newElement;
        }
        else {
            head = newElement;
            tail = newElement;
        }
    }

    T* popData(){
        if (!head) {
            return nullptr;
        }

        Element* returnElement = head;
        head = head->next;

        if (!head){
            tail = nullptr;
        }

        T* returnData = returnElement->data;
        kMemoryAllocator::getInstance().mem_free(returnElement);
        return returnData;
    }

    bool find(T* data){
        for (Element* current = head; current != nullptr; current = current->next) {
            if (current->data == data)
                return true;
        }

        return false;
    }

    T* removeElement(T* data){
        Element* previous = nullptr;

        for (Element* current = head; current != nullptr; current = current->next){
            if (current->data == data) {
                if (current == head){
                    head = current->next;
                    if (!head)
                        tail = nullptr;
                }
                else if (current == tail){
                    tail = previous;
                    tail->next = nullptr;
                }
                else {
                    previous->next = current->next;
                }

                T* return_data = current->data;
                kMemoryAllocator::getInstance().mem_free(current);
                return return_data;
            }

            previous = current;
        }

        return nullptr;
    }

};

#endif