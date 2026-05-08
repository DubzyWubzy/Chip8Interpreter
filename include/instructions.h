//
// Created by Zach White on 5/8/26.
//

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>


/*
// clear screen
void ins_00E0();

// jump
void ins_1NNN();

// set register VX
void ins_6XNN();

// add value to register VX
void ins_6XNN();

// set index register I
void ins_ANNN();

// draw
void ins_DXYN();
*/

void instruction_execute(uint16_t instruction); // also does the decoding

