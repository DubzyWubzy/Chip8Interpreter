#pragma once

#include <stdlib.h>
//#include <stdint.h>

#include <pthread.h>
#include <stdatomic.h>

// BELOW CONSTS ARE INTERNAL FOR THE DISPLAY
#define WIDTH  832
#define HEIGHT 416

#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32
#define SCALE 13


#define STACK_SIZE 16 // for 16 2-byte entires






extern uint32_t buffer[];
uint32_t displayBuffer[WIDTH * HEIGHT]; // DISPLAY

static pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
static atomic_int running = 1;

extern uint8_t systemMemory[]; // MEMORY

extern uint8_t oldCpuRegisters[];


/* INDEXES:
* 0-1 = PC (Program Counter (for instructions))
 * 2-3 = I (Index Counter (for memory))
 * 4 = Delay Timer
 * 5 = Sound Timer
 * 6-21 = V0-VF GP registers
*/



struct registerStruct
{
    uint16_t programCounter;
    uint16_t indexPointer;
    uint8_t delayTimer;
    uint8_t soundTimer;

    uint8_t V[16]; // GP registers
};
extern struct registerStruct cpuRegisters;

struct stackStruct
{
    uint16_t stack[STACK_SIZE];// array where each item is 16 bits
    int topOfStack; // just the index of the current top of the stack
};

extern struct stackStruct callStack;



