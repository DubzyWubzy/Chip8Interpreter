#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STACK_SIZE = 16; // for 16 2-byte entires

struct callStack
{
    uint16_t stack[16];// array where each item is 16 bits
    void* topOfStack; // just the address of the current top of the stack
    // capacity (may not need this...?)
};

void initializeFont(uint16_t *systemMemory)
{
    uint16_t tempIndexRegister = 0x50; // fonts go from 0x050 to 0x09F

    // gonna read this from a document, font.txt
    FILE* fontFile = fopen("../font.txt", "r");
    if (!fontFile) {
        perror("fopen");
    }

    char line[1024]; // this buffer does NOT need to be this big
    while (fgets(line, 100, fontFile))
    {
        char* token = strtok(line, ", ");
        // five values per line
        for (int i = 0; i < 5; i++)
        {
            if (token == NULL) {break;}

            systemMemory[tempIndexRegister] = strtoul(token, NULL, 16);

            tempIndexRegister += 0x1;
            token = strtok(NULL, ", ");
        }
    }

    fclose(fontFile);

}


int main(void)
{
    // declare the Chip-8 RAM space
    uint16_t systemMemory[4096]; // as the index register is 16 bits, each
    // entry in the memory will be as well
    // 12 bit index pointer, the maximum of this is 4096
    
    initializeFont(systemMemory);



}
