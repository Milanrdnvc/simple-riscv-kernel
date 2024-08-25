#include "../h/printing.hpp"
#include "../h/syscall_c.h"

void userMain() {
    int x = 10;
    x++;
    printString("TEST");
    getc();
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < 100000; j++) {

        }
    }
//    for (int i = 0; i < 10; i++) {
//        printString("This is just a test for user flow of control.\n");
//    }
}