#include "../h/cons.hpp"
#include "../h/printing.hpp"
#include "../h/tcb.hpp"
#include "../h/memoryAllocator.hpp"
#include "../h/consoleBuffer.hpp"

ConsoleBuffer* Cons::outputBuffer = new ConsoleBuffer(256);
ConsoleBuffer* Cons::inputBuffer = new ConsoleBuffer(256);

void Cons::consPutc(char c) {
    outputBuffer->put(c);
}

void Cons::putcThr(void* arg) {
    // polling
    for (int i = 0; i < 10; i++) {
        printString("PUTC THREAD TEST\n");
        if (i == 5) {
            __asm__ volatile("csrr t6, sepc");
            printString("PUTC THREAD EXECUTED CSRR\n");
        }
    }
}

int Cons::startPutcThr(thread_t* handle) {
    char* stackSpace = (char*)MemoryAllocator::memAlloc(DEFAULT_STACK_SIZE);
    if (!stackSpace) return -1;
    TCB* thr = new TCB(putcThr, nullptr, stackSpace, true);
    if (!thr) {
        *handle = nullptr;
        return -1;
    }

    *handle = thr;
    return 0;
}
