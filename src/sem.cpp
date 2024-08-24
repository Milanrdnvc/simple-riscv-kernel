#include "../h/sem.hpp"
#include "../h/scheduler.hpp"

List<Sem> Sem::allSemaphores;

int Sem::semWait() {
    if (--val < 0) block();
    if (this->closed) return -1;
    return 0;
}

int Sem::semSignal() {
    if (++val <= 0) unblock();
    return 0;
}

void Sem::block() {
    TCB* old = TCB::runningThread;
    blocked.addLast(old);
    TCB::runningThread = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::runningThread->context);
}

void Sem::unblock() {
    TCB* t = blocked.removeFirst();
    if (t) Scheduler::put(t);
}

int Sem::semTryWait() {
    if (this->value() - 1 >= 0) {
        this->val--;
        return 0;
    }
    return 1;
}

int Sem::semTimedWait(time_t timeout) {
    if (--val < 0) {
        this->timeout = timeout;
        block();
    }
    if (this->closed) return -1;
    if (this->timedOut) return -2;
    return 0;
}

int Sem::createSemaphore(Sem** handle, unsigned init) {
    Sem* sem = new Sem(init);
    if (!sem) return -1;
    *handle = sem;
    Sem::allSemaphores.addFirst(sem);
    return 0;
}

int Sem::closeSemaphore(Sem *id) {
    if (id->closed) return -1;
    id->closed = true;

    while(id->blocked.peekFirst()) {
        Scheduler::put(id->blocked.removeFirst());
    }

    return 0;
}


