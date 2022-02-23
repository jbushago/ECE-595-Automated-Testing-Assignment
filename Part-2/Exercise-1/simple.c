#include <stdio.h>
#include <stdlib.h>

void simple(int x) {
    int* evil = (void *) 0;
    if (x % 21 == 0){
        *evil = x;
    }
    if (x == 0xBADBAD)
        *evil = x;
    return;
}

int main() {
    int a;
    if (scanf("%d", &a) != 1)
        return EXIT_FAILURE;
    simple(a);
    return EXIT_SUCCESS;
}
