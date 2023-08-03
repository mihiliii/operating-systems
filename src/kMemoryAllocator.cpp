#include "../h/kMemoryAllocator.h"

kMemoryAllocator::kMemoryAllocator() {
    head = (FreeMem*) CONST_HEAP_START_ADDR;
    head->size = ((size_t) HEAP_END_ADDR - (size_t) CONST_HEAP_START_ADDR);
    head->next = nullptr;
}

//Pozvana kernel metoda ce datu velicinu size, koja je izrazena u bajtovima, prebaciti u blokove
//a zatim pozvati mem_alloc, pri cemu ce se standardno alocirati memoriju.
void* kMemoryAllocator::kmem_alloc(size_t size) {
    size += CONST_FREEMEM_SIZE;

    if (size % MEM_BLOCK_SIZE) {
        size = size / MEM_BLOCK_SIZE + 1;
    }
    else {
        size /= MEM_BLOCK_SIZE;
    }

    return mem_alloc(size);
}

void* kMemoryAllocator::mem_alloc(size_t size) {
    size_t new_size = size * MEM_BLOCK_SIZE;
    FreeMem* previous = nullptr;
    FreeMem* current = head;

    while (current){

        if (new_size <= current->size){
            FreeMem* retBlock = current;

            if (new_size < current->size) {

                if (current == head) {
                    head = (FreeMem*)((char*)current + new_size);
                }
                else {
                    previous->next = (FreeMem*)((char*)current + new_size);
                }

                current = (FreeMem*)((char*)current + new_size);
                current->size = retBlock->size - new_size;
                current->next = retBlock->next;
            }

            else if (new_size == current->size){

                if (current == head) {
                    head = head->next;
                }
                else {
                    previous->next = current->next;
                }

            }

            retBlock->next = nullptr;
            retBlock->size = new_size;
            return (retBlock + 1);
        }

        previous = current;
        current = current->next;
    }

    return nullptr;
}

int kMemoryAllocator::mem_free(void* ptr) {
    FreeMem* freeBlock = (FreeMem*) ptr - 1;

    if (freeBlock < CONST_HEAP_START_ADDR || freeBlock >= HEAP_END_ADDR){
        return -1;
    }

    if (head == nullptr){
        head = freeBlock;
        return 0;
    }

    FreeMem* current = head;

    while (current->next != nullptr && current->next < freeBlock) {
        current = current->next;
    }

    //ako je freeBlock odmah iznad pokazivaca current onda se spaja
    //sa current pokazivacem u jedan veci fragment
    if ((FreeMem*) ((char*) current + current->size) == freeBlock){
        current->size += freeBlock->size;

        if ((FreeMem*) ((char*) current + current->size) == current->next){
            current->size += current->next->size;
            current->next = current->next->next;
        }

        return 0;
    }

    //ako je freeBlock odmah ispod pokazivaca current->next onda se spaja
    //sa current->next pokazivacem u jedan veci fragment
    if ((FreeMem*) ((char*) freeBlock + freeBlock->size) == current->next){
        freeBlock->size += current->next->size;
        freeBlock->next = current->next->next;
        current->next = freeBlock;
        return 0;
    }

    //ako je freeBlock odmah ispod pokazivaca head onda se spaja
    //sa head pokazivacem u jedan veci fragment
    else if ((FreeMem*) ((char*) freeBlock + freeBlock->size) == head){
        freeBlock->size += head->size;
        freeBlock->next = head->next;
        head = freeBlock;
        return 0;
    }

    //dodavanje bez spajanja
    if (freeBlock < head){
        freeBlock->next = head;
        head = freeBlock;
    }
    else {
        freeBlock->next = current->next;
        current->next = freeBlock;
    }

    return 0;
}

kMemoryAllocator &kMemoryAllocator::getInstance() {
    static kMemoryAllocator memoryAllocator;
    return memoryAllocator;
}
