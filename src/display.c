#include "display.h"

#include "system.h"


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

bool check_logical_pixel(int x, int y)
{
    return (buffer[(y * WIDTH + x) * SCALE]) == 0xFFFFFFFF;
}

void resetScreen() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        buffer[i] = 0x00000000;
}

// TODO: fill screen function?

void printHexChar(const uint8_t hexToPrint, const int initialX, int initialY)
{
    uint16_t tempIndexRegister = (0x50 + (hexToPrint * 0x5));

    if (hexToPrint < 0x0 || hexToPrint > 0xF) {printf("%s\n", "Invalid character to print"); return;}

    for (int i = 0; i < 5; i++)
    {
        uint8_t currentLine = (systemMemory[tempIndexRegister]/16); // the /16 is to make the 8 bit number
        //into the four bit number relevant for font data

        for (int j = 0; j < 4; j++) {
            if ((currentLine & (1 << (3-j))))
            {
                set_logical_pixel(initialX + j, (initialY + i), true);
            }
        }
        tempIndexRegister += 0x1;
    }
}

// for n rows...
// get nth byte of sprite data from memory[indexCounter], iC not incremented
// for each of the 8 pixels from left to right:
//  if current pixel in sprite row AND display itself is on, turn pixel off and set VF to 1
//  OR if the current sprite pixel is on and screen pixel isnt, draw the pixel
//  if right edge of screen reached, stop drawing this row
//  increment x
void printSprite(const int initialX, const int initialY, const int rowCount)
{
    // set VF to 0:
    cpuRegisters.V[0xF] = 0;

    uint16_t tempIndexRegister = cpuRegisters.indexPointer;

    for (int i = 0; i < rowCount; i++)
    {
        uint8_t currentLine = (systemMemory[tempIndexRegister]); // can this be made const?

        for (int j = 0; j < 8; j++) {
            if ((currentLine & (1 << (7-j)))) // TODO: make this more descriptive
            {
                // here, we're in the pixel itself. Now we check to see if the pixel is alrady on
                if (check_logical_pixel(initialX + j, initialY + i))
                {
                    set_logical_pixel(initialX + j, (initialY + i), true);
                    cpuRegisters.V[0xF] = 1;
                } else {
                    set_logical_pixel(initialX + j, (initialY + i), false);
                }
            }
        }
        tempIndexRegister += 0x1;
    }
}

int updateWindow(struct mfb_window *window)
{

    mfb_update_state state;

    do {

        // TODO: fix the logical pixels so that the rounded corners don't
        // cut them off on Mac
        // maybe using a cute little border or something?

        state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

        if (state != MFB_STATE_OK)
            break;

    } while(mfb_wait_sync(window));


    return 0;

}


