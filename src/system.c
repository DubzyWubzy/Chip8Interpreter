//
// Created by Zach White on 5/8/26.
//

#include "system.h"

uint32_t buffer[WIDTH * HEIGHT]; // DISPLAY

uint8_t systemMemory[4096]; // MEMORY, 4096 8-bit blocks

struct registerStruct cpuRegisters = {0x0};
