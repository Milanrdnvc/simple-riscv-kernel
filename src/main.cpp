#include "../h/memoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"

void userMain();

void userMainTests();

void userMainWrapper(void* arg) {
//    userMain();
    userMainTests();
}

void main() {
    // initialize the kernel
    MemoryAllocator::memInit();
    RISCV::w_stvec((size_t)(&RISCV::interruptRoutine));

    thread_t threads[2];
    thread_create(&threads[0], nullptr, nullptr);
    TCB::runningThread = threads[0]; // must be init. imm. after we create a thread for main
    thread_create(&threads[1], &userMainWrapper, nullptr);

    RISCV::ms_sstatus(RISCV::SSTATUS_SIE);

    while (!threads[1]->getFinished()) {
        thread_dispatch();
    }

    for (auto &thread : threads) {
        delete thread;
    }

    RISCV::halt();
}