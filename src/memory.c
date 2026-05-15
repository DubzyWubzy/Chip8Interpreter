#include "memory.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "system.h"


int loadProgram(char programFile[])
{
    const int fptr = open(programFile, O_RDONLY);

    if (fptr==-1) {printf("%s\n","Invalid program file"); return 1;}



    size_t r = read(fptr, systemMemory + 0x200, 4096 - 0x200); // ANOTHER case of the magic number

    if (r == -1) {printf("%s\n","ERROR WHEN COPYING PROGRAM TO MEMORY!");}

    close(fptr);
    return 0;
}

void initializeFont()
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
    printf("%s", "Font loaded into memory!\n");

}

void stackPush(uint16_t toBePushed) { callStack.stack[callStack.topOfStack] = toBePushed; callStack.topOfStack += 1; }

uint16_t stackPop() { callStack.topOfStack -= 1; return callStack.stack[(callStack.topOfStack + 1)]; }

