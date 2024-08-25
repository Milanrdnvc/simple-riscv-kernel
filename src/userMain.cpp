#include "../h/printing.hpp"
#include "../h/syscall_c.h"

void userMain() {
    for (int i = 0; i < 10; i++) {
        printString("This is just a test for user flow of control.\n");
    }
}