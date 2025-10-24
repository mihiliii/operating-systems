#include "../h/kScheduler.h"

kQueue<kTCB> kScheduler::queue_ready_threads;

kTCB* kScheduler::get()
{
    return queue_ready_threads.popData();
}

void kScheduler::put(kTCB* object)
{
    queue_ready_threads.pushData(object);
}

bool kScheduler::isEmpty()
{
    return queue_ready_threads.isEmpty();
}

kTCB* kScheduler::removeElement(kTCB* object)
{
    return queue_ready_threads.removeElement(object);
}
