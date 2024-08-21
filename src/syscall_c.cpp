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
