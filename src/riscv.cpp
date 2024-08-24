#include "../h/riscv.hpp"
#include "../h/memoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/printing.hpp"
#include "../h/sem.hpp"
#include "../h/scheduler.hpp"

void RISCV::popSppSpie() {
     mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void RISCV::systemPopSppSpie() {
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

        // logic for sem_timedwait
        Sem::allSemaphores.setFirst();
        while (Sem::allSemaphores.getCurrent()) {
            if (Sem::allSemaphores.getCurrent()->timeout < 0) continue;
            Sem::allSemaphores.getCurrent()->timeout--;
            if (Sem::allSemaphores.getCurrent()->timeout == 0) {
                while (Sem::allSemaphores.getCurrent()->blocked.peekFirst()) {
                    Scheduler::put(Sem::allSemaphores.getCurrent()->blocked.removeFirst());
                }
                Sem::allSemaphores.getCurrent()->timedOut = true;
            }
            Sem::allSemaphores.next();
        }

        // logic for time_sleep
        TCB::sleepingThreads.setFirst();
        while (TCB::sleepingThreads.getCurrent()) {
            TCB::sleepingThreads.getCurrent()->timeSleeping--;
            if (TCB::sleepingThreads.getCurrent()->timeSleeping == 0) {
                Scheduler::put(TCB::sleepingThreads.removeCurrent());
            }
            TCB::sleepingThreads.next();
        }

        // asynchronous context switch
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= DEFAULT_TIME_SLICE) {
            size_t volatile sepc = r_sepc();
            size_t volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sepc(sepc);
            w_sstatus(sstatus);
            w_scause(scause);
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
            // sem_open
            case 0x21: {
                Sem** handle = (Sem**)arg1;
                unsigned init = (unsigned)arg2;
                int retVal = Sem::createSemaphore(handle, init);

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // sem_close
            case 0x22: {
                Sem* id = (Sem*)arg1;
                int retVal = Sem::closeSemaphore(id);
                delete id;

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // sem_wait
            case 0x23: {
                Sem* id = (Sem*)arg1;
                int retVal = id->semWait();

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // sem_signal
            case 0x24: {
                Sem* id = (Sem*)arg1;
                int retVal = id->semSignal();

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // sem_timedwait
            case 0x25: {
                Sem* id = (Sem*)arg1;
                time_t timeout = (time_t)arg2;
                int retVal = id->semTimedWait(timeout);

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            // sem_trywait
            case 0x26: {
                Sem* id = (Sem*)arg1;
                int retVal = id->semTryWait();

                RISCV::returnSysCall((size_t)retVal);
                break;
            }
            case 0x31: {
                time_t time = (time_t)arg1;
                TCB::timeSleep(time);
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
        w_scause(scause);
    } else {
        // unexpected trap cause
        size_t volatile sepc = r_sepc();
        printString("Exception! scause: ");
        printInt(scause);
        printString("\n sepc: ");
        printInt(sepc);
        while (true) {}
    }
}

