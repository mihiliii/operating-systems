#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.h"

void* operator new(size_t);
void* operator new[](size_t);
void operator delete(void*) noexcept;
void operator delete[](void*) noexcept;

class Thread {
public:

    Thread(void (*function_body)(void*), void* arg);
    virtual ~Thread();

    int start();

    static void dispatch();

    static void runFunctionWrapper(void* obj);

protected:

    Thread();
    virtual void run() {}

private:

    thread_t myHandle;
};

class Semaphore {
public:

    Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:

    sem_t myHandle;
};

class Console {
public:

    static char getc();
    static void putc(char);
};

#endif
