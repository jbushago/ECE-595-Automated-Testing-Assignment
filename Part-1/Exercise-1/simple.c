#include "klee.h"

#include <stdlib.h>

void simple(long x) {
    int* a = malloc(10);
    if (x == 0xBADBAD)
        free(a);
    if (x != 0xA150BAD)
        free(a);
    return;
}

int main() {
    long a;
    klee_make_symbolic(&a, sizeof(a), "a");
    simple(a);
    return EXIT_SUCCESS;
} 