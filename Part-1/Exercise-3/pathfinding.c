#include "klee.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERS 25
#define HEIGHT 10
#define WIDTH 11

char MAZE[HEIGHT][WIDTH] = {
    "+-+-------+",
    "| |       |",
    "| | +-+ + |",
    "| + | | | |",
    "|   | | | |",
    "| + + | | |",
    "| |   | | |",
    "| +-+ | +-+",
    "|   | |  #|",
    "+---+-+---+"
};

void print_maze() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", MAZE[i][j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

bool traverse_maze(char* path) {
    int x = 1;
    int y = 1;
    for(; *path; path++) {
        MAZE[y][x] = 'X';
        switch (*path) {
        case 'w':
            y--;
            break;
        case 'a':
            x--;
            break;
        case 's':
            y++;
            break;
        case 'd':
            x++;
            break;
        }
        if (MAZE[y][x] == '#') {
            MAZE[y][x] = 'X';
            print_maze();
            return true;
        } else if (MAZE[y][x] != ' ') {
            return false;
        }
    }
    return false;
}

int main() {
    char path[MAX_ITERS];
    /*
     * Make path symbolic using `klee_make_symbolic`,
     * then use `klee_assume` to guarantee that path
     * is null-terminated.
     * Finally, `assert` that a call to `traverse_maze` with
     * your symbolic path will not return true.
     * Does KLEE successfully find the end of the maze?
     */
    return EXIT_SUCCESS;
}
