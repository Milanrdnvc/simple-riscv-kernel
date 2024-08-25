#ifndef _cons_hpp_
#define _cons_hpp_

#include "consoleBuffer.hpp"
#include "syscall_c.h"

class Cons {
public:
    static void consPutc(char c);
    static char consGetc();

    static int startPutcThr(thread_t* handle);
    static int startGetcThr(thread_t* handle);
private:
    static ConsoleBuffer* outputBuffer;
    static ConsoleBuffer* inputBuffer;

    static void putcThr(void*);
    static void getcThr(void*);
};

#endif