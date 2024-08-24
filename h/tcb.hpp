#ifndef _tcb_hpp_
#define _tcb_hpp_

#include "../lib/hw.h"
#include "../h/memoryAllocator.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"

class Sem;

// this implementation was taken from "OS1 Vezbe 7 zadatak 6 riscv context switch 1 synchronous"
class TCB {
public:
    static TCB* runningThread;

    using Body = void(*)(void*);

    bool getFinished() const { return finished; }
    void setFinished(bool aFinished) { this->finished = aFinished; }

    static int createThread(TCB** handle, Body body = nullptr, void* arg = nullptr, char* stackSpace = nullptr);

    static void dispatch();

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

    ~TCB();
private:
    TCB(Body body, void* arg, char* stackSpace);

    struct Context {
        size_t ra;
        size_t sp;
    };

    static size_t timeSliceCounter;

    Body body;
    char* stack;
    Context context;
    bool finished;
    void* arg;
    time_t timeSleeping;

    static List<TCB> sleepingThreads;

    static void timeSleep(time_t time);

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void yield();

    static void threadWrapper();

    friend class RISCV;
    friend class Sem;
};

#endif
