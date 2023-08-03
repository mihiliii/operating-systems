#ifndef kMemoryAllocator_h
#define kMemoryAllocator_h

#include "../lib/hw.h"

//Singleton klasa kMemoryAllocator koja sluzi za alokaciju memorije.
//Radi na osnovu first-fit algoritma.

class kMemoryAllocator {

    struct FreeMem {
        size_t size;
        FreeMem* next;
    };

    const uint64 CONST_FREEMEM_SIZE = 16;
    const void* CONST_HEAP_START_ADDR = (void*) ((size_t) HEAP_START_ADDR + MEM_BLOCK_SIZE -
            ((size_t) HEAP_START_ADDR % MEM_BLOCK_SIZE));

    FreeMem* head;

    kMemoryAllocator();

public:

    kMemoryAllocator(const kMemoryAllocator&) = delete;
    void operator=(const kMemoryAllocator&) = delete;

    static kMemoryAllocator& getInstance();

    //Postoje 2 metode za alokaciju prostora. kmem_alloc sluzi da se poziva unutar kernela
    //pri cemu se izbegava ponovni sistemski poziv dok se procesor nalazi u sistemskom rezimu

    void* kmem_alloc (size_t size);

    void* mem_alloc (size_t size);

    int mem_free (void* ptr);

};


#endif