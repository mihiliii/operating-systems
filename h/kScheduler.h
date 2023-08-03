#ifndef kScheduler_h
#define kScheduler_h

#include "kQueue.h"

class kTCB;

class kScheduler{
public:

    static kTCB* get();

    static void put(kTCB*);

    static bool empty();

    static bool find(kTCB*);

    static kTCB* removeElement(kTCB*);

private:

    static kQueue<kTCB> queue_readyThreads;

};

#endif
