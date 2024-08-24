#ifndef _consoleBuffer_hpp_
#define _consoleBuffer_hpp_

#include "sem.hpp"

class ConsoleBuffer {
public:
    ConsoleBuffer(int cap);
    ~ConsoleBuffer();

    void put(char c);
    char get();

private:
    int cap;
    char *buffer;
    int head, tail;

    Sem* spaceAvailable;
    Sem* itemAvailable;
    Sem* mutexHead;
    Sem* mutexTail;
};

#endif
