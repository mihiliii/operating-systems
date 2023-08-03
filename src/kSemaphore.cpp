#include "../h/kSemaphore.h"

kQueue<kSemaphore> kSemaphore::semaphore_list;

int kSemaphore::wait() {
    this->value--;

    if (this->value < 0){
        kTCB* old = kTCB::running_thread;
        old->setStatus(kTCB::TS_SUSPENDED);
        blockedQueue.pushData(old);

        kTCB::running_thread = kScheduler::get();
        kTCB::contextSwitch(&old->context, &kTCB::running_thread->context);
    }

    return 0;
}

int kSemaphore::signal() {
    this->value++;

    if (this->value <= 0){
        kTCB* tcb = blockedQueue.popData();
        tcb->setStatus(kTCB::TS_READY);
        kScheduler::put(tcb);
    }

    return 0;
}

int kSemaphore::createSemaphore(kSemaphore** handle, unsigned init) {
    *handle = new kSemaphore(init);
    return 0;
}

int kSemaphore::closeSemaphore(kSemaphore* handle) {
    while (!handle->blockedQueue.isEmpty()){
        kScheduler::put(handle->blockedQueue.popData());
    }

    kSemaphore::semaphore_list.removeElement(handle);
    delete handle;
    return 0;
}

void *kSemaphore::operator new(size_t size) {
    return ((kSemaphore*) kMemoryAllocator::getInstance().kmem_alloc(size));
}

void kSemaphore::operator delete(void *ptr) {
    kMemoryAllocator::getInstance().mem_free(ptr);
}