#include "../h/syscall_c.h"

void passArgumentsToSyscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 a4);

void* mem_alloc(size_t size) {
    const int FREEMEM_SIZE = 16;
    size += FREEMEM_SIZE;
    if (size % MEM_BLOCK_SIZE){
        size = size / MEM_BLOCK_SIZE + 1;
    }
    else {
        size /= MEM_BLOCK_SIZE;
    }

    passArgumentsToSyscall(0x01, size, 0, 0, 0);

    void* return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return (void*) return_variable;
}

int mem_free(void *ptr) {
    passArgumentsToSyscall(0x02, (uint64) ptr, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

int thread_create(thread_t* handle, void (*start_routine)(void *), void *arg) {
    void *ptr_stack = nullptr;

    if (start_routine != nullptr) {
        size_t size = DEFAULT_STACK_SIZE * sizeof(uint64);
        ptr_stack = mem_alloc(size);
    }

    passArgumentsToSyscall(0x11, (uint64) handle, (uint64) start_routine,
                           (uint64) arg, (uint64) ptr_stack);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

int thread_exit() {
    passArgumentsToSyscall(0x12, 0, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

void thread_dispatch() {
    passArgumentsToSyscall(0x13, 0, 0, 0, 0);
}

int sem_open(sem_t* handle, unsigned int init) {
    passArgumentsToSyscall(0x21, (uint64) handle, init, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

int sem_close(sem_t handle) {
    passArgumentsToSyscall(0x22, (uint64) handle, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

int sem_wait(sem_t id) {
    passArgumentsToSyscall(0x23, (uint64) id, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

int sem_signal(sem_t id) {
    passArgumentsToSyscall(0x24, (uint64) id, 0, 0, 0);

    int return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

char getc(){
    passArgumentsToSyscall(0x41, 0, 0, 0, 0);

    char return_variable;
    asm volatile("mv %[returnVariableASM], a0" : [returnVariableASM] "=r" (return_variable));
    return return_variable;
}

void putc(char c){
    passArgumentsToSyscall(0x42, (uint64) c, 0, 0, 0);
}