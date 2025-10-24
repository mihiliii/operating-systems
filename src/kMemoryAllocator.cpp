#include "../h/kMemoryAllocator.h"

kMemoryAllocator::kMemoryAllocator()
{
    head = (size_t) HEAP_START_ADDR % 64 == 0
               ? (FreeMem*) HEAP_START_ADDR
               : (FreeMem*) ((size_t) HEAP_START_ADDR + (64 - ((size_t) HEAP_START_ADDR % 64)));
    head->size = ((size_t) HEAP_END_ADDR - (size_t) head);
    head->next = nullptr;
}

/**
 * Kernel metoda koja datu velicinu size, izrazenu u bajtovima, prebacuje u broj blokova
 * a zatim poziva mem_alloc, pri cemu ce se standardno alocirati memoriju.
 */
void* kMemoryAllocator::kmem_alloc(size_t size)
{
    size += sizeof(FreeMem);

    if (size % MEM_BLOCK_SIZE != 0) {
        size = size / MEM_BLOCK_SIZE + 1;
    }
    else {
        size /= MEM_BLOCK_SIZE;
    }

    return mem_alloc(size);
}

void* kMemoryAllocator::mem_alloc(size_t size_in_blocks)
{
    size_t size = size_in_blocks * MEM_BLOCK_SIZE;
    FreeMem* current = head;
    FreeMem* previous = current;

    while (current) {
        if (size <= current->size) {
            FreeMem* return_block = current;

            if (size < current->size) {
                if (current == head) {
                    head = (FreeMem*) ((char*) current + size);
                }
                else {
                    previous->next = (FreeMem*) ((char*) current + size);
                }

                current = (FreeMem*) ((char*) current + size);
                current->size = return_block->size - size;
                current->next = return_block->next;
            }

            else if (size == current->size) {
                if (current == head) {
                    head = head->next;
                }
                else {
                    previous->next = current->next;
                }
            }

            return_block->next = nullptr;
            return_block->size = size;
            return (return_block + 1);
        }

        previous = current;
        current = current->next;
    }

    return nullptr;
}

int kMemoryAllocator::mem_free(void* ptr)
{
    FreeMem* free_block = (FreeMem*) ptr - 1;

    if (free_block < HEAP_START_ADDR || free_block >= HEAP_END_ADDR) {
        return -1;
    }

    if (head == nullptr) {
        head = free_block;
        return 0;
    }

    FreeMem* current = head;

    while (current->next != nullptr && current->next < free_block) {
        current = current->next;
    }

    // ako je free_block odmah iznad pokazivaca current onda se spaja
    // sa current pokazivacem u jedan veci fragment
    if ((FreeMem*) ((char*) current + current->size) == free_block) {
        current->size += free_block->size;

        if ((FreeMem*) ((char*) current + current->size) == current->next) {
            current->size += current->next->size;
            current->next = current->next->next;
        }

        return 0;
    }

    // ako je free_block odmah ispod pokazivaca current->next onda se spaja
    // sa current->next pokazivacem u jedan veci fragment
    if ((FreeMem*) ((char*) free_block + free_block->size) == current->next) {
        free_block->size += current->next->size;
        free_block->next = current->next->next;
        current->next = free_block;
        return 0;
    }

    // ako je free_block odmah ispod pokazivaca head onda se spaja
    // sa head pokazivacem u jedan veci fragment
    else if ((FreeMem*) ((char*) free_block + free_block->size) == head) {
        free_block->size += head->size;
        free_block->next = head->next;
        head = free_block;
        return 0;
    }

    // dodavanje bez spajanja
    if (free_block < head) {
        free_block->next = head;
        head = free_block;
    }
    else {
        free_block->next = current->next;
        current->next = free_block;
    }

    return 0;
}

kMemoryAllocator& kMemoryAllocator::instance()
{
    static kMemoryAllocator memory_allocator;
    return memory_allocator;
}
