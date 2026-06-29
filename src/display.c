#include "display.h"
#include "system.h"

#include <MiniFB.h>
#include <string.h>
#include <unistd.h>



mfb_update_state state;

// non-display related MiniFB functions:
int map_key(char key)
{
    switch (key)
    {
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0xC;
    case 'Q': return 0x4;
    case 'W': return 0x5;
    case 'E': return 0x6;
    case 'R': return 0xD;
    case 'A': return 0x7;
    case 'S': return 0x8;
    case 'D': return 0x9;
    case 'F': return 0xE;
    case 'Z': return 0xA;
    case 'X': return 0x0;
    case 'C': return 0xB;
    case 'V': return 0xF;
    default: return -1;
    }
}

// a function like this should be moved WAY out of display ngl
static void keyboard(struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool is_pressed) {
    pressed_key = map_key(key);
    // TODO: if pressed_key = -1, abort

    if (is_pressed == false) {
        //printf("%s", "RELEASE");
        //printf("%i", pressed_key);
        keystate &= ~(1 << pressed_key);
        // we keep this example code in so the user can close the window...can we also make it so that it ends the program?
        if (key == MFB_KB_KEY_ESCAPE) {
            mfb_close(window);
        }
    } else
    {
        //printf("%s", "PRESS");
        //printf("%i", pressed_key);
        keystate |= (1 << pressed_key);
    }
    //printf("%X\n", keystate);
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
                    set_logical_pixel(initialX + j, (initialY + i), false);
                    cpuRegisters.V[0xF] = 1;
                } else {
                    set_logical_pixel(initialX + j, (initialY + i), true);
                }
            }
        }
        tempIndexRegister += 0x1;
    }
}


void *drawThread(void *arg)
{

    while (atomic_load(&running))
    {
        pthread_mutex_lock(&buffer_mutex);
        memcpy(displayBuffer, buffer, (WIDTH * HEIGHT)); // is there a more efficient way?
        pthread_mutex_unlock(&buffer_mutex);

        if (cpuRegisters.delayTimer < 61) {cpuRegisters.delayTimer -= 1;
        // for the timer registers in the cpu...god this is disorganized wtf is this
        usleep(16666);
    }
    return NULL;
}

int initWindow(struct mfb_window *window)
{
    mfb_set_keyboard_callback(window, keyboard);
    return 0;
}

int updateWindow(struct mfb_window *window)
{
    do {
        // TODO: fix the logical pixels so that the rounded corners don't
        // cut them off on Mac
        // maybe using a cute little border or something?
        pthread_mutex_lock(&buffer_mutex);
        //state = mfb_update_ex(window, displayBuffer, WIDTH, HEIGHT);
        state = mfb_update_ex(window, buffer, WIDTH, HEIGHT);

        pthread_mutex_unlock(&buffer_mutex);

        if (state != MFB_STATE_OK)
        {
            atomic_store(&running, 0);
            break;
        }
        usleep(16666); // to achieve 60fps
    } while(mfb_wait_sync(window));

    return 0;

}

