#ifndef _memoryAllocator_hpp_
#define _memoryAllocator_hpp_

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void* memAlloc(size_t size); // size in bytes
    static int memFree(void* startAddr);
    static void memInit();

private:
    struct FreeMem {
        FreeMem* next;
        size_t size; // size in blokcs
    };

    static void tryToMerge(FreeMem* fragment);

    static FreeMem* freeHead;
};

#endif
