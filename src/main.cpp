#include "../h/ABI.h"
#include "../h/kTCB.h"
#include "../h/kSemaphore.h"

void userMain();

void userMainWrapper(void* arg){
    userMain();
}

typedef kTCB* thread_t;

void main(){

    thread_t mainT, userMainT;
    auto stack_ptr = (uint64*) kMemoryAllocator::getInstance().kmem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));

    kTCB::createThread(&mainT, nullptr, nullptr, nullptr, true);
    kTCB::createThread(&userMainT, &userMainWrapper, nullptr, stack_ptr, true);

    asm volatile("csrw stvec, %[supervisorTrapASM]"
            : : [supervisorTrapASM] "r" (&supervisorTrap));

    bool break_condition;
    do {
        break_condition = true;
        kTCB::yield();
        for (auto iterator = kTCB::list_threads.getIterator(); iterator != nullptr; iterator = iterator->next){
            kTCB* tcb = iterator->data;
            if (tcb->getStatus() != kTCB::TS_FINISHED){
                break_condition = false;
            }
            else {
                kTCB::deleteThread(tcb);
            }
        }
    }
    while (!break_condition);

    while (kSemaphore* sem = kSemaphore::semaphore_list.popData()){
        kSemaphore::closeSemaphore(sem);
    }

    while (kTCB* tcb = kTCB::list_threads.popData()){
        kTCB::deleteThread(tcb);
    }
    kTCB::deleteThread(mainT);

}