#ifndef _syscall_c_h_
#define _syscall_c_h_

#include "../lib/hw.h"

void* mem_alloc(size_t size);
int mem_free(void* startAddr);

class TCB;
typedef TCB* thread_t;
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t* handle);
void thread_setId(thread_t thr, int id);
int thread_getId(thread_t thr);

class Sem;
typedef Sem* sem_t;
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal (sem_t id);
int sem_timedwait(sem_t id, time_t timeout);
int sem_trywait(sem_t id);
void sem_add_owner(sem_t id);
void sem_remove_owner(sem_t id, int thrId);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc ();
void putc (char);

#endif
