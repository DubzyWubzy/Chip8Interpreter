//
// Created by Zach White on 5/1/26.
//

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "memory.h"


/* INDEXES:
 * 0-1 = PC (Program Counter
 * 2-3 = I (Instruction Counter)
 * 4 = Delay Timer
 * 5 = Sound Timer
 * 6-21 = V0-VF GP registers
*/


// TODO: Implement timers


void *FDE(void *arg);