#ifndef kSemaphore_h
#define kSemaphore_h

#include "../lib/hw.h"
#include "Queue.h"
#include "kTCB.h"

class kSemaphore {
public:

    int wait();

    int signal();

    static int createSemaphore(kSemaphore** handle, unsigned init);

    static int closeSemaphore(kSemaphore* handle);

    void* operator new(size_t size);

    void operator delete(void* ptr);

    static Queue<kSemaphore*> queue_semaphores;

private:

    explicit kSemaphore(uint32 init = 1) : value((int) init) {
        queue_semaphores.push(this);
    }

    int value;
    Queue<kTCB*> queue_blocked_threads;
};

#endif