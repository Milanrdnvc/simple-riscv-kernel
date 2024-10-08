#include "../h/tcb.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"

TCB* TCB::runningThread = nullptr;

size_t TCB::timeSliceCounter = 0;

List<TCB> TCB::sleepingThreads;

TCB::TCB(Body body, void *arg, char *stackSpace, bool systemMode) {
    this->body = body;
    this->stack = stackSpace;
    this->arg = arg;
    this->finished = false;
    this->context = {
            !systemMode ? (size_t)threadWrapper : (size_t)systemThreadWrapper,
            stack ? (size_t)&stack[DEFAULT_STACK_SIZE] : 0
    };
    if (body) Scheduler::put(this);
}

TCB::~TCB() {
    MemoryAllocator::memFree(stack);
}

void TCB::timeSleep(time_t time) {
    TCB::runningThread->timeSleeping = time;
    TCB::sleepingThreads.addFirst(TCB::runningThread);

    TCB* old = TCB::runningThread;
    TCB::runningThread = Scheduler::get();
    contextSwitch(&old->context, &TCB::runningThread->context);
}

void TCB::dispatch() {
    TCB* old = runningThread;
    if (!old->getFinished()) Scheduler::put(old);
    runningThread = Scheduler::get();

    contextSwitch(&old->context, &runningThread->context);
}

void TCB::yield() {
    __asm__ volatile("ecall");
}

int TCB::createThread(TCB** handle, Body body, void* arg, char* stackSpace) {
    TCB* newThread = new TCB(body, arg, stackSpace, false);
    if (!newThread) {
        *handle = nullptr;
        return -1;
    }
    *handle = newThread;
    return 0;
}

void TCB::threadWrapper() {
    RISCV::popSppSpie();
    runningThread->body(runningThread->arg);
    runningThread->setFinished(true);
    RISCV::w_a0(0x13);
    yield();
}

void TCB::systemThreadWrapper() {
    RISCV::systemPopSppSpie();
    runningThread->body(runningThread->arg);
    runningThread->setFinished(true);
    RISCV::w_a0(0x13);
    yield();
}

bool TCB::schedulerIsEmpty() {
    return Scheduler::peekFirst() == nullptr;
}
