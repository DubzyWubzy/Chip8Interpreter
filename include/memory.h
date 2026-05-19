// This header file defines the memory portion of the emulator, both the
// 4KB of main system ram and the stack structure.

#pragma once

#include <stdlib.h> // TODO: put this in the cmakelists
#include <stdio.h>
#include <string.h>
#include <stdint.h>



int loadProgram(char programFile[]);

void initializeFont();

// TODO: maybe put these in system?
void stackPush(uint16_t toBePushed);

uint16_t stackPop();