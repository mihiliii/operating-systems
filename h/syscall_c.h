#ifndef syscall_c_h
#define syscall_c_h

#include "../lib/hw.h"

class kTCB;
typedef kTCB* thread_t;

void* mem_alloc(size_t size);

int mem_free(void* ptr);

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

class kSemaphore;
typedef kSemaphore* sem_t;

int sem_open(sem_t* handle, unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal(sem_t id);

char getc();

void putc(char);

#endif