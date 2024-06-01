#ifndef kTCB_h
#define kTCB_h

#include "kMemoryAllocator.h"
#include "kScheduler.h"

class kTCB {
public:

    enum Thread_Status {
        TS_INITIALIZED,
        TS_RUNNING,
        TS_READY,
        TS_SUSPENDED,
        TS_FINISHED
    };

    friend class kSemaphore;

    friend void main();

    using BodyFunction = void (*)(void*);

    static kTCB* running_thread;

    static Queue<kTCB*> queue_threads;

    //--methods--//

    Thread_Status getStatus() const {
        return status;
    }

    void setStatus(Thread_Status thread_status) {
        this->status = thread_status;
    }

    static int createThread(
        kTCB** tcb, void (*body)(void*), void* args, uint64* ptr_stack, bool start
    );

    static int startThread(kTCB* tcb);

    static void yield();

    static void dispatch();

    static int exitThread();

    static int deleteThread(kTCB* tcb);

    ~kTCB() {
        kMemoryAllocator::instance().mem_free(stack);
    }

private:

    struct Context {
        uint64 reg_ra;
        uint64 reg_sp;
    };

    BodyFunction function_body;
    void* arguments;
    uint64* stack;
    Context context;
    Thread_Status status;

    static void popSppSpieRegisters();

    static void threadWrapper();

    void* operator new(size_t size);

    void operator delete(void* ptr);

    static void contextSwitch(kTCB::Context*, kTCB::Context*);

    kTCB(BodyFunction body, void* args, uint64* ptr_stack, bool start = true) :
        function_body(body),
        arguments(args),
        stack(body != nullptr ? ptr_stack : nullptr),
        context(
            {(uint64) &threadWrapper, (stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0)}
        ),
        status(TS_INITIALIZED) {
        if (body != nullptr) {
            if (start) {
                kScheduler::put(this);
                status = TS_READY;
            }
            queue_threads.push(this);
        }
        if (kTCB::running_thread == nullptr) {
            kTCB::running_thread = this;
            kTCB::running_thread->status = TS_RUNNING;
        }
    }
};

#endif