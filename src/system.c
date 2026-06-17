
#include "system.h"

#include <stdint.h>


uint32_t buffer[WIDTH * HEIGHT]; // gets copied to diplay 60 times a second
uint32_t displayBuffer[WIDTH * HEIGHT]; // DISPLAY

uint8_t systemMemory[4096]; // MEMORY, 4096 8-bit blocks
// TODO: another magic number to get rid of

struct registerStruct cpuRegisters = {0x0};

struct stackStruct callStack = {0x0};

/*1 2 3 c
 *4 5 6 d
 *7 8 9 e
 *a 0 b f
 *equals...
 *1 2 3 4
 *q w e r
 *a s d f
 *z x c v
 */
char virtualKeyboard[4][4] = {
    {'1','2','3','C'},
    {'4','5','6','D'},
    {'7','8','9','E'},
    {'A','0','B','F'}
};



char pressed_key; // do we need this?

void changePressedKey(char key)
{
    // screw it let's just do a damn switch case
}

// we use this as a buffer between display (where miniFB lives and where keyboard
// input will be taken, and processor which will use this keyboard input data.
// enum is another way we can go about this...