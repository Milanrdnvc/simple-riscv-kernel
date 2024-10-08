#ifndef _printing_hpp_
#define _printing_hpp_

#include "syscall_c.h"

typedef unsigned long uint64;

extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);

void printString(char const *string);

void printStringS(char const *string);

char* getString(char *buf, int max);

int stringToInt(const char *s);

void printInt(int xx, int base=10, int sgn=0);

void printIntS(int xx, int base=10, int sgn=0);

#endif

