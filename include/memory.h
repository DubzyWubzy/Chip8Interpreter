// This header file defines the memory portion of the emulator, both the
// 4KB of main system ram and the stack structure.

#pragma once

#include <stdlib.h> // TODO: put this in the cmakelists
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define STACK_SIZE 16 // for 16 2-byte entires




struct callStack
{
    uint16_t stack[STACK_SIZE];// array where each item is 16 bits
    void* topOfStack; // just the address of the current top of the stack
    // capacity (may not need this...?)
};



// declare the Chip-8 RAM space
// 12 bit index pointer, the maximum of this is 4096


void initializeFont(uint8_t *systemMemory);