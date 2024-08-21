#include "../h/syscall_c.h"
#include "../h/riscv.hpp"

void sysCall(size_t arg0, size_t arg1 = 0, size_t arg2 = 0, size_t arg3 = 0, size_t arg4 = 0, size_t arg5 = 0) {
    __asm__ volatile("ecall");
}

void* mem_alloc(size_t size) {
    if (size <= 0) return nullptr;
    size_t sizeInBlocks = (size / MEM_BLOCK_SIZE) + (size % MEM_BLOCK_SIZE == 0 ? 0 : 1);
    sysCall(0x01, sizeInBlocks);
    void* volatile retVal = (void*)RISCV::r_a0();
    return retVal;
}

int mem_free(void* startAddr) {
    if (!startAddr) return -1;
    sysCall(0x02, (size_t)startAddr);
    int volatile retVal = (int)RISCV::r_a0();
    return retVal;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    if (!handle) return -1;
    void* stackSpace = nullptr;
    if (start_routine) {
        stackSpace = mem_alloc(DEFAULT_STACK_SIZE);
        if (!stackSpace) return -1;
    }
    sysCall(0x11, (size_t)handle, (size_t)start_routine, (size_t)arg, (size_t)stackSpace);
    int volatile retVal = (int)RISCV::r_a0();
    return retVal;
}

void thread_dispatch() {
    sysCall(0x13);
}

int thread_exit() {
    sysCall(0x12);
    int volatile retVal = (int)RISCV::r_a0();
    return retVal;
}