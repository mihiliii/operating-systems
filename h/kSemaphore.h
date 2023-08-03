#ifndef kSemaphore_h
#define kSemaphore_h

#include "../lib/hw.h"
#include "kTCB.h"
#include "kQueue.h"

class kSemaphore{
public:

    int wait();

    int signal();

    static int createSemaphore(kSemaphore** handle, unsigned init);

    static int closeSemaphore(kSemaphore* handle);

    void* operator new(size_t size);

    void operator delete(void* ptr);

    static kQueue<kSemaphore> semaphore_list;

private:

    explicit kSemaphore(unsigned init = 1) : value((int) init) {
        semaphore_list.pushData(this);
    }

    int value;
    kQueue<kTCB> blockedQueue;

};

#endif