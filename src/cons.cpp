#include "../h/cons.hpp"

ConsoleBuffer* Cons::outputBuffer;
ConsoleBuffer* Cons::inputBuffer;

void Cons::consPutc(char c) {
    outputBuffer->put(c);
}

void Cons::putcThr(void* arg) {
    while (true) {
        // polling
        while (!(*((char*)CONSOLE_STATUS) & 0x20)) {}
        *((char*)CONSOLE_TX_DATA) = outputBuffer->get();
    }
}

int Cons::startPutcThr(thread_t* handle) {
    outputBuffer = new ConsoleBuffer(256);

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
