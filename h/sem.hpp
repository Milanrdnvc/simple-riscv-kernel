#ifndef _sem_hpp_
#define _sem_hpp_

#include "../h/list.hpp"
#include "../h/tcb.hpp"

class Sem {
public:
    Sem (unsigned init): val(init) {}

    int semWait();
    int semSignal();

    int value() const { return val; }

    int semTryWait();

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
    List<TCB> blocked;

    void block();
    void unblock();
};

#endif
