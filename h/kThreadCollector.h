
#ifndef kThreadCollector_h
#define kThreadCollector_h

#include "Queue.h"
#include "kTCB.h"

class kThreadCollector {
private:

    Queue<kTCB*> queue_thread_collector;

public:

    kThreadCollector() : queue_thread_collector() {}

    void run() {
        while (true) {
            while (!queue_thread_collector.isEmpty()) {

            }
        }
    }

    void put(kTCB* thread) {
        queue_thread_collector.push(thread);
    }

};


#endif
