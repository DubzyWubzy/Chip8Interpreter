#include <stdio.h>
#include <stdlib.h>

const int STACK_SIZE = 16; // for 16 2-byte entires

struct callStack
{
    uint16_t stack[16];// array where each item is 16 bits
    // top
    // capacity
};


int main(void)
{
    int* systemMemoryStart = malloc(512); // declare the Chip-8 RAM space
    printf("%p", systemMemoryStart);




    free(systemMemoryStart); // clean everything up at the end
}
