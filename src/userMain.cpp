#include "../lib/console.h"

void userMain() {
    for (int i = 0; i < 10; i++) {
        __putc('a');
        __putc('\n');
    }
}