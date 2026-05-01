//
// Created by Zach White on 5/1/26.
//

#pragma once

#include <stdlib.h>

uint8_t cpuRegisters[22] = {0}; // the CPU's PC, will be moved to its own little place soon

/* INDEXES:
 * 0-1 = PC (Program Counter
 * 2-3 = I (Instruction Counter)
 * 4 = Delay Timer
 * 5 = Sound Timer
 * 6-21 = V0-VF GP registers
*/