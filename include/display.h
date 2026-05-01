#pragma once

#define WIDTH  832
#define HEIGHT 416

#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32
#define SCALE 13

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> // why didn't i need this in main...
#include <MiniFB.h>

// BELOW CONSTS ARE INTERNAL FOR THE DISPLAY



uint32_t buffer[WIDTH * HEIGHT]; // this is the main GUI window buffer


// helper functions for drawWindow
void set_pixel(int x, int y, bool on);
void set_logical_pixel(int x, int y, bool on);

void reset();

// we may need to change this function to printSprite in accordance to the DXYN command
void printHexChar(const uint8_t *systemMemory, uint8_t hexToPrint, int initialX, int initialY);

int drawWindow(uint8_t *systemMemory);