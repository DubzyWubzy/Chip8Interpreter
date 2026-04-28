#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int* systemMemoryStart = malloc(512); // declare the Chip-8 RAM space
    printf("%p", systemMemoryStart);


    
    free(systemMemoryStart); // clean everything up at the end
}
