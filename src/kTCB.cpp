#include "../h/kTCB.h"

kTCB* kTCB::running_thread = nullptr;
kQueue<kTCB> kTCB::list_threads;

extern "C" void pushAllRegisters();

extern "C" void popAllRegisters();

extern int thread_exit();

int kTCB::createThread(kTCB** tcb, void (*body)(void*), void* args, uint64* ptr_stack, bool start) {
    *tcb = new kTCB(body, args, ptr_stack, start);
    return 0;
}

int kTCB::startThread(kTCB* tcb){
    if (tcb->getStatus() == TS_INITIALIZED){
        tcb->setStatus(TS_READY);
        kScheduler::put(tcb);
        return 0;
    }

    return -1;
}

void kTCB::yield() {
    pushAllRegisters();

    kTCB::dispatch();

    popAllRegisters();
}

void kTCB::dispatch() {
    kTCB* old = running_thread;

    if (old->getStatus() != TS_FINISHED && old->getStatus() != TS_SUSPENDED){
        old->setStatus(TS_READY);
        kScheduler::put(old);
    }

    kTCB::running_thread = kScheduler::get();
    kTCB::running_thread->setStatus(TS_RUNNING);

    contextSwitch(&old->context, &running_thread->context);
}

int kTCB::exitThread() {
    running_thread->setStatus(TS_FINISHED);
    kTCB::dispatch();
    return 0;
}

int kTCB::deleteThread(kTCB* tcb) {
    kScheduler::removeElement(tcb);
    kTCB::list_threads.removeElement(tcb);
    delete tcb;
    return 0;
}

void kTCB::popSppSpieRegisters() {
    asm volatile("csrw sepc, ra\n\t"
                 "sret"
                 );
}

void kTCB::threadWrapper() {
    kTCB::popSppSpieRegisters();
    running_thread->function_body(running_thread->arguments);
    thread_exit();
}

void* kTCB::operator new(size_t size) {
    return ((kTCB*) kMemoryAllocator::getInstance().kmem_alloc(size));
}

void kTCB::operator delete(void* ptr) {
    kMemoryAllocator::getInstance().mem_free((kTCB*) ptr);
}


