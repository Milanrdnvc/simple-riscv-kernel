#include "../h/riscv.hpp"
#include "../h/memoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/printing.hpp"

void RISCV::popSppSpie() {
     mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void RISCV::handleInterruptRoutine() {
    size_t volatile scause = r_scause();
    size_t volatile code = r_a0();
    size_t volatile arg1 = r_a1();
    size_t volatile arg2 = r_a2();
    size_t volatile arg3 = r_a3();
    size_t volatile arg4 = r_a4();

    if (scause == 0x8000000000000001UL) {
        // timer interrupt
        mc_sip(SIP_SSIP);

        TCB::timeSliceCounter++;

        if (TCB::timeSliceCounter >= DEFAULT_TIME_SLICE) {
            size_t volatile sepc = r_sepc();
            size_t volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
           // TCB::dispatch();
            w_sepc(sepc);
            w_sstatus(sstatus);
        }

    } else if (scause == 0x8000000000000009UL) {
        // console interrupt
        console_handler();
    } else if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // ecall from system/user mode
        size_t volatile sepc = r_sepc() + 4;
        size_t volatile sstatus = r_sstatus();
        switch (code) {
            // mem_alloc
            case 0x01: {
                size_t volatile sizeInBytes = r_a1() * MEM_BLOCK_SIZE;
                void* retVal = MemoryAllocator::memAlloc(sizeInBytes);

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // mem_free
            case 0x02: {
                void *volatile startAddr = (void *) r_a1();
                int retVal = MemoryAllocator::memFree(startAddr);

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // thread_create
            case 0x11: {
                TCB **threadHandlePtr = (TCB**)arg1;
                TCB::Body startRoutine = (TCB::Body)arg2;
                void *arg = (void *)arg3;
                char *stackSpace = (char *)arg4;
                int retVal = TCB::createThread(threadHandlePtr, startRoutine, arg, stackSpace);

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // thread_exit
            case 0x12: {
                TCB::runningThread->setFinished(true);
                TCB::dispatch();
                break;
            }
            // thread_dispatch
            case 0x13: {
                TCB::dispatch();
                break;
            }
            // getc
            case 0x41: {
                char retVal = __getc();

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // putc
            case 0x42: {
                char c = (char)arg1;
                __putc(c);
                break;
            }
            // unsupported system call code
            default: {
                printString("Unsupported system call code.");
                break;
            }
        }

        w_sepc(sepc);
        w_sstatus(sstatus);
    } else {
        // unexpected trap cause
        size_t volatile sepc = r_sepc();
        size_t volatile scause = r_scause();
        printString("Exception! scause: ");
        printInt(scause);
        printString("\n sepc: ");
        printInt(sepc);
        while (true) {}
    }
}

