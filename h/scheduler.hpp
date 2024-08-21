#ifndef _scheduler_hpp_
#define _scheduler_hpp_

#include "list.hpp"

class TCB;

// this implementation was taken from "OS1 Vezbe 7 zadatak 6 riscv context switch 1 synchronous"
class Scheduler {
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);
};

#endif