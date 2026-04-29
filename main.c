#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MiniFB.h>

#define WIDTH  832
#define HEIGHT 416

#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32
#define SCALE 13

uint32_t buffer[WIDTH * HEIGHT];


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
    printf("%s", "Font loaded into memory!\n");

}

// helper functions for drawWindow
void set_pixel(int x, int y, bool on)
{
    uint32_t color;
    if (on) {color = 0xFFFFFFFF;} else {color = 0x00000000;}

    if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
    {
        buffer[y * WIDTH + x] = color;
    }
}

void set_logical_pixel(int x, int y, bool on) {
    if ((x >= 0) && (x < LOGICAL_WIDTH) && (y >= 0) && (y < LOGICAL_HEIGHT)) {
        for (int dy = 0; dy < SCALE; dy++)
            for (int dx = 0; dx < SCALE; dx++)
                set_pixel(x * SCALE + dx, y * SCALE + dy, on);
    }
}

void reset() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        buffer[i] = 0x00000000;
}


int drawWindow()
{
    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, MFB_WF_RESIZABLE);

    mfb_update_state state;
    do {
        // step 1: turn the pixels into logical pixels
        set_logical_pixel(0, 0, true);
        set_logical_pixel(63, 31, true);
        set_logical_pixel(0, 31, true);
        set_logical_pixel(63, 0, true);


        state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));

    window = NULL;

    return 0;

}



int main(void)
{
    // declare the Chip-8 RAM space
    uint16_t systemMemory[4096]; // as the index register is 16 bits, each
    // entry in the memory will be as well
    // 12 bit index pointer, the maximum of this is 4096

    initializeFont(systemMemory);

    drawWindow();



}
