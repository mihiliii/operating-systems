#include "../h/ABI.h"
#include "../h/kSemaphore.h"
#include "../h/kTCB.h"

void userMain();

void userMainWrapper(void* arg)
{
    userMain();
}

typedef kTCB* thread_t;

int main()
{
    thread_t mainT, userMainT;
    auto stack_ptr =
        (uint64*) kMemoryAllocator::instance().kmem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));

    kTCB::createThread(&mainT, nullptr, nullptr, nullptr, true);
    kTCB::createThread(&userMainT, &userMainWrapper, nullptr, stack_ptr, true);

    asm volatile("csrw stvec, %[supervisorTrapASM]" : : [supervisorTrapASM] "r"(&supervisorTrap));

    bool break_condition;
    do {
        break_condition = true;
        kTCB::yield();
        for (auto iterator = kTCB::queue_threads.getIterator(); iterator != nullptr;
             iterator = iterator->next) {
            kTCB* tcb = iterator->data;
            if (tcb->getStatus() != kTCB::TS_FINISHED) {
                break_condition = false;
            }
            else {
                kTCB::deleteThread(tcb);
            }
        }
    } while (!break_condition);

    while (kSemaphore* sem = kSemaphore::queue_semaphores.popData()) {
        kSemaphore::closeSemaphore(sem);
    }

    while (kTCB* tcb = kTCB::queue_threads.popData()) {
        kTCB::deleteThread(tcb);
    }
    kTCB::deleteThread(mainT);

    return 0;
}
