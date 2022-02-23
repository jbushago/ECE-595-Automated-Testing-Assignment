#include "json.h"
#include "klee.h"

#define SIZE 7

int main() {
    char json[SIZE];
    /*
     * Make json symbolic using `klee_make_symbolic`,
     * then use `klee_assume` to guarantee that path
     * is null-terminated (as described in the README).
     */
    return EXIT_SUCCESS;
}
