#ifndef _cons_hpp_
#define _cons_hpp_

#include "consoleBuffer.hpp"
#include "syscall_c.h"

class Cons {
public:
    static void consPutc(char c);
    static char consGetc();

    static int startPutcThr(thread_t* handle);
    static void setGetcBuffer();

    static void inputBufferPut(char c);
    static ConsoleBuffer* getOutputBuffer();
private:
    static ConsoleBuffer* outputBuffer;
    static ConsoleBuffer* inputBuffer;

    static void putcThr(void*);
    static void getcThr(void*);
};

#endif