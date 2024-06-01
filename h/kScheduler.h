#ifndef kScheduler_h
#define kScheduler_h

#include "Queue.h"

class kTCB;

class kScheduler {
public:

    static kTCB* get();

    static void put(kTCB*);

    static bool isEmpty();

    static kTCB* removeElement(kTCB*);

private:

    static Queue<kTCB*> queue_ready_threads;
};

#endif
