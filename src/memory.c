#include "memory.h"

void initializeFont(uint8_t *systemMemory)
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