#include "../h/syscall_cpp.h"

void passArgumentsToSyscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4);

void* operator new(size_t size)
{
    return mem_alloc(size);
}

void* operator new[](size_t size)
{
    return mem_alloc(size);
}

void operator delete(void* ptr) noexcept
{
    mem_free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    mem_free(ptr);
}

Thread::Thread(void (*function_body)(void*), void* arg)
{
    myHandle = nullptr;
    void* ptr_stack = nullptr;

    if (function_body != nullptr) {
        size_t alloc_size = DEFAULT_STACK_SIZE * sizeof(uint64);
        ptr_stack = mem_alloc(alloc_size);
    }

    passArgumentsToSyscall(0x101, (uint64) & this->myHandle, (uint64) &function_body, (uint64) arg,
                           (uint64) ptr_stack);
}

Thread::~Thread() = default;

int Thread::start()
{
    passArgumentsToSyscall(0x103, (uint64) this->myHandle, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r"(return_variable));
    return return_variable;
}

void Thread::dispatch()
{
    thread_dispatch();
}

Thread::Thread()
{
    myHandle = nullptr;
    size_t alloc_size = DEFAULT_STACK_SIZE * sizeof(uint64);
    void* ptr_stack = mem_alloc(alloc_size);

    passArgumentsToSyscall(0x101, (uint64) & this->myHandle, (uint64) &runFunctionWrapper,
                           (uint64) this, (uint64) ptr_stack);
}

void Thread::runFunctionWrapper(void* obj)
{
    if (obj) {
        ((Thread*) obj)->run();
    }
}

Semaphore::Semaphore(unsigned int init)
{
    this->myHandle = nullptr;
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(this->myHandle);
}

int Semaphore::wait()
{
    return sem_wait(this->myHandle);
}

int Semaphore::signal()
{
    return sem_signal(this->myHandle);
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}
