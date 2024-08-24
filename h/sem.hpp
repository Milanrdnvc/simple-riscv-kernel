#ifndef _sem_hpp_
#define _sem_hpp_

#include "list.hpp"
#include "tcb.hpp"

class Sem {
public:
    Sem (unsigned init): val(init), closed(false), timedOut(false), timeout(-1) {}

    int semWait();
    int semSignal();

    int value() const { return val; }

    int semTryWait();

    int semTimedWait(time_t timeout);

    static int createSemaphore(Sem** handle, unsigned init);

    static int closeSemaphore(Sem* id);

    void* operator new(size_t size) {
        return MemoryAllocator::memAlloc(size);
    }

    void* operator new[](size_t size) {
        return MemoryAllocator::memAlloc(size);
    }

    void operator delete(void *ptr) {
        MemoryAllocator::memFree(ptr);
    }

    void operator delete[](void *ptr) {
        MemoryAllocator::memFree(ptr);
    }

private:
    int val;
    bool closed;
    bool timedOut;
    time_t timeout;
    List<TCB> blocked;

    static List<Sem> allSemaphores;

    static int test;

    void block();
    void unblock();

    friend class RISCV;
};

#endif
