#ifndef kTCB_h
#define kTCB_h

#include "kScheduler.h"
#include "kMemoryAllocator.h"

class kTCB{
public:

    enum THREAD_STATUS {
        TS_INITIALIZED, TS_RUNNING, TS_READY, TS_SUSPENDED, TS_FINISHED
    };

    friend class kSemaphore;

    friend void main();

    using BodyFunction = void (*)(void*);

    static kTCB* running_thread;

    static kQueue<kTCB> list_threads;

    //--methods--//

    THREAD_STATUS getStatus() const { return status; }

    void setStatus(THREAD_STATUS Status) { this->status = Status; }

    static int createThread(kTCB** tcb, void (*body)(void *), void* args, uint64* ptr_stack, bool start);

    static int startThread(kTCB* tcb);

    static void yield();

    static void dispatch();

    static int exitThread();

    static int deleteThread(kTCB* tcb);

    ~kTCB() {
        kMemoryAllocator::getInstance().mem_free(stack);
    }

private:

    struct Context{
        uint64 raRegister;
        uint64 spRegister;
    };

    BodyFunction function_body;
    void* arguments;
    uint64* stack;
    Context context;
    THREAD_STATUS status;

    static void popSppSpieRegisters();

    static void threadWrapper();

    void* operator new(size_t size);

    void operator delete(void* ptr);

    static void contextSwitch(kTCB::Context*, kTCB::Context*);

    kTCB(BodyFunction body, void* args, uint64* ptr_stack, bool start = true) :
            function_body(body),
            arguments(args),
            stack(body != nullptr ? ptr_stack : nullptr),
            context({(uint64) &threadWrapper,
                     (stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0)}),
            status(TS_INITIALIZED)
            {
                if (body != nullptr){
                    if (start){
                        kScheduler::put(this);
                        status = TS_READY;
                    }
                    list_threads.pushData(this);
                }
                if (kTCB::running_thread == nullptr){
                    kTCB::running_thread = this;
                    kTCB::running_thread->status = TS_RUNNING;
                }
            }

};

#endif