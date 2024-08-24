#include "../h/consoleBuffer.hpp"
#include "../h/syscall_cpp.hpp"

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
    sem_wait(spaceAvailable);

    sem_wait(mutexTail);
    buffer[tail] = c;
    tail = (tail + 1) % cap;
    sem_signal(mutexTail);

    sem_signal(itemAvailable);

}

char ConsoleBuffer::get() {
    sem_wait(itemAvailable);

    sem_wait(mutexHead);

    int ret = buffer[head];
    head = (head + 1) % cap;
    sem_signal(mutexHead);

    sem_signal(spaceAvailable);

    return ret;
}
