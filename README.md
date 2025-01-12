# Dynamic Memory Allocator

This program simulates dynamic memory allocation in C. It parses `stdin` input, splits it into *words* (groups of characters separated by spaces), and saves all words into a toy memory space represented by the following structure:

```c
struct space {
    char *memory;
    int len;
};
```
where `memory` points to a dynamically allocated memory block of size len (allocated during memory initialisation). 

The allocator scans the array to find a free memory block of the requested size. If successful, it returns the index of the start of the block. 

The pointers to the allocated blocks are stored in a linked list with nodes described by the following structure:

```c
struct node {
    int p;
    struct node *next;
};
```
where `p` describes the position of a block in memory. 

The list has one node for each input word and grows dynamically at runtime.

## Running the program

To run the program, compile the `example.c` file using the gcc compiler

`example.c` is some example code that makes use of the `functions.c` library and contains a loop that removes all words shorter than REMOVE characters when the user presses `return`. 

It also adds `BATCH` bytes (the space for 10 characters) to the memory when needed.

Run the binary using the command:
```
./a.out < filename.txt
```

where `filename.txt` is any text file.


