#include "../h/kScheduler.h"

Queue<kTCB*> kScheduler::queue_ready_threads;

kTCB* kScheduler::get() {
    return queue_ready_threads.pop();
}

void kScheduler::put(kTCB* object) {
    queue_ready_threads.push(object);
}

bool kScheduler::isEmpty() {
    return queue_ready_threads.isEmpty();
}

kTCB* kScheduler::removeElement(kTCB* object) {
//    return queue_ready_threads.removeElement(object);
    return nullptr;
}
