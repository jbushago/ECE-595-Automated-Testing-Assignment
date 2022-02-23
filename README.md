# Automated Testing with KLEE and AFL

## Part 1: KLEE

### Setup KLEE
We'll be using a docker image to simplify dependency management.
In the root directory of this assignment, run the following commands to
pull the docker image and set up some "plumbing" between the container and
this directory in your filesystem.

```{bash}
$ docker pull klee/klee:2.1
$ docker run --rm -ti -v $PWD:/home/klee/hw  --ulimit='stack=-1:-1' klee/klee:2.1
```

### Exercise 1.1
This first exercise will familiarize you with the basic process of using KLEE.

Before running any commands, open up the file `Exercise-1/simple.c`.
Inspect the function `void simple(long x)` -- there are two possible memory
errors in this function. What are they?

In our `main` function notice the call to `klee_make_symbolic`. This instructs
KLEE to represent our variable symbolically. We then execute the `simple`
function on this symbolic input.

In your docker terminal, run the following to compile `simple.c` into LLVM
bitcode and run KLEE on the output. Does KLEE find both memory errors?

```{bash}
$ cd hw/Part-1/Exercise-1
$ clang -I /home/klee/klee_src/include/klee -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone simple.c
$ klee -output-dir ./klee simple.bc
```

### Exercise 1.2

Exercise 1 was a good introduction to KLEE, but the error it found was trivial
to see in a code review. This exercise will focus on a more realistic scenario.

The file `Exercise-2/json.h` is a copy of an open source json parser written
in C. Give it a quick skim, this is the type of code that is hard to verify
by inspection alone. In `klee_json.c` we'll utilize KLEE to test the
`json_parse` function defined in `json.h`.

To test this function, we need to create a symbolic null-terminate string.
KLEE is unable to symbolically represent the size of an array, so we
need to choose a constant size to use. For this exercise we've defined
the size of the string to be 7 bytes, but keep in mind that such a small
string might not trigger all possible errors.

Creating a symbolic null-terminated string with KLEE is fairly straightforward.
First, make the array symbolic with `klee_make_symbolic` like so:
```{C}
klee_make_symbolic(json, sizeof(json), "json");
```

This symbolic array is not necessarily null-terminated! To guarantee that
we're only testing with null-terminated strings, we can use `klee_assume` to
instruct KLEE to operate as though the last element in the array is 0.
```{C}
klee_assume(json[SIZE - 1] == '\0');
```

After creating the symbolic null-terminated string, run your code in docker
with the following commands (don't worry about any KLEE warnings).
```{bash}
$ cd ../Exercise-2
$ clang -I /home/klee/klee_src/include/klee -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone klee_json.c
$ klee -output-dir ./klee klee_json.bc
```

This may take some time, if all goes well we should find a memory error. What
line does this error occur on?

### Exercise 1.3

Symbolic execution, in comparison to fuzzing, execls at determining if code is
reachable. Where a fuzzer might not find a case that triggers a specific
branch, symbolic execution will inspect all possible branches (although it
will miss other problems that the fuzzer might catch). In this exercise, we
take great advantage of these properties and force KLEE to solve a maze.

Inspect `Exercise-3/pathfinding.c`, on a very high level you should see that
the `traverse_maze` function parses a null-terminated string as a list of
commands for traversing the maze defined at the top of the file. If the
traversed path leads to the end of the maze (denoted by `#`), then the
function returns `true`.

In the `main` function, create a symbolic null-terminated string and assert
`traverse_maze`, when run with that symbolic string as input, will return
`false`. If done correctly, KLEE will be able to traverse the maze and find
the exit!

In your docker terminal, run the following to test your code.

```{bash}
$ cd ../Exercise-3
$ clang -I /home/klee/klee_src/include/klee -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone pathfinding.c
$ klee -output-dir ./klee --warnings-only-to-file pathfinding.bc # supressing warnings to make printout readable
```

## Part 2: AFL

### Step 1: Setup AFL

In order to set up AFL, first exit docker with `exit`,
then navigate to the root directory of this assignment and run the following:
```bash
$ git submodule init
$ git submodule update
$ cd AFL
$ make
$ export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 # required to run on this VM
$ export AFL_USE_ASAN=1 # check for memory corruption
$ export PATH=$PATH:$(pwd)
$ export AFL_PATH=$(pwd)
$ cd ..
```

### Exercise 2.1
This exercise will familiarize you with the basic process of using AFL.

Before running any commands, open up the file `Exercise-1/simple.c`.
Inspect the function `void simple(int x)` -- just like in exercise 1.1, there
are two possible memory errors in this function. What are they?

In our `main` function, notice how we read the input from `stdin`. AFL
interacts with our program through `stdin` so we need to build a "test
harness" which uses `stdin` as input to the function under test.

To fuzz this code, run the following in your bash terminal. Wait until AFL
has completed at least 300 cycles. How many of the memory errors did it find?
Inspect the `out/crashes` folder to determine which error it found.

```{bash}
$ cd Part-2/Exercise-1
$ afl-gcc -g simple.c -o simple
$ afl-fuzz -i in -o out -m none ./simple
```

### Exercise 2.2
Now that you've used AFL on a given example, you'll be writing your own
test harness for differential testing.

Open up the file `Exercise-2/diff.c` and note the two seemingly semantically
identical implementations of the [SQRDMLAH](https://developer.arm.com/documentation/ddi0596/2020-12/SIMD-FP-Instructions/SQRDMLAH--by-element---Signed-Saturating-Rounding-Doubling-Multiply-Accumulate-returning-High-Half--by-element--)
instruction.

Write a test harness which reads two `int16_t`s from `stdin` and asserts that
both implementations of the function return the same value.

Then run the following in bash to perform differential fuzzing.
Do you know why one of the implementations is wrong?

```{bash}
$ cd Part-2/Exercise-2
$ afl-gcc -g diff.c -o diff
$ afl-fuzz -i in -o out -m none ./diff
```
