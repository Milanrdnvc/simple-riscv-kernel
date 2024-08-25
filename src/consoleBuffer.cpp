#include "../h/consoleBuffer.hpp"

ConsoleBuffer::ConsoleBuffer(int cap) : cap(cap), head(0), tail(0) {
    buffer = new char[cap];
    Sem::createSemaphore(&itemAvailable, 0);
    Sem::createSemaphore(&spaceAvailable, cap);
    Sem::createSemaphore(&mutexHead, 1);
    Sem::createSemaphore(&mutexTail, 1);
}

ConsoleBuffer::~ConsoleBuffer() {
    delete buffer;
    Sem::closeSemaphore(itemAvailable);
    Sem::closeSemaphore(spaceAvailable);
    Sem::closeSemaphore(mutexHead);
    Sem::closeSemaphore(mutexHead);
    delete itemAvailable;
    delete spaceAvailable;
    delete mutexHead;
    delete mutexTail;
}

void ConsoleBuffer::put(char c) {
    spaceAvailable->semWait();
    mutexTail->semWait();
    buffer[tail] = c;
    tail = (tail + 1) % cap;
    mutexTail->semSignal();
    itemAvailable->semSignal();
}

char ConsoleBuffer::get() {
    itemAvailable->semWait();
    mutexHead->semWait();
    char ret = buffer[head];
    head = (head + 1) % cap;
    mutexHead->semWait();
    spaceAvailable->semSignal();
    return ret;
}
