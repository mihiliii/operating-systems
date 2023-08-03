#include "../h/kScheduler.h"

kQueue<kTCB> kScheduler::queue_readyThreads;

kTCB *kScheduler::get() {
    return queue_readyThreads.popData();
}

void kScheduler::put(kTCB* object) {
    queue_readyThreads.pushData(object);
}

bool kScheduler::empty() {
    return queue_readyThreads.isEmpty();
}

bool kScheduler::find(kTCB* object) {
    return queue_readyThreads.find(object);
}

kTCB *kScheduler::removeElement(kTCB* object) {
    return queue_readyThreads.removeElement(object);
}
