#include "../h/kSemaphore.h"

kQueue<kSemaphore> kSemaphore::queue_semaphores;

int kSemaphore::wait() {
    this->value--;

    if (this->value < 0) {
        kTCB* old = kTCB::running_thread;
        old->setStatus(kTCB::TS_SUSPENDED);
        queue_blocked_threads.pushData(old);

        kTCB::running_thread = kScheduler::get();
        kTCB::contextSwitch(&old->context, &kTCB::running_thread->context);
    }

    return 0;
}

int kSemaphore::signal() {
    this->value++;

    if (this->value <= 0) {
        kTCB* tcb = queue_blocked_threads.popData();
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
    while (!handle->queue_blocked_threads.isEmpty()) {
        kScheduler::put(handle->queue_blocked_threads.popData());
    }

    kSemaphore::queue_semaphores.removeElement(handle);
    delete handle;
    return 0;
}

void* kSemaphore::operator new(size_t size) {
    return ((kSemaphore*) kMemoryAllocator::instance().kmem_alloc(size));
}

void kSemaphore::operator delete(void* ptr) {
    kMemoryAllocator::instance().mem_free(ptr);
}