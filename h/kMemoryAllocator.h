#ifndef kMemoryAllocator_h
#define kMemoryAllocator_h

#include "../lib/hw.h"

/**
 * Singleton klasa koja sluzi za alokaciju prostora koristeci first-fit algoritam.
 */
class kMemoryAllocator {

    struct FreeMem {
        size_t size;
        FreeMem* next;
    };

    FreeMem* head;

    kMemoryAllocator();

public:

    kMemoryAllocator(const kMemoryAllocator&) = delete;
    void operator=(const kMemoryAllocator&) = delete;

    static kMemoryAllocator& instance();

    // Postoje 2 metode za alokaciju prostora. kmem_alloc sluzi da se poziva unutar kernela
    // pri cemu se izbegava ponovni sistemski poziv dok se procesor nalazi u sistemskom rezimu

    void* kmem_alloc(size_t size);

    void* mem_alloc(size_t size);

    int mem_free(void* ptr);
};

#endif
