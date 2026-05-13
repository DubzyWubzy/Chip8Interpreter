#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> // why didn't i need this in main...
#include <MiniFB.h>




// helper functions for drawWindow
void set_pixel(int x, int y, bool on);
void set_logical_pixel(int x, int y, bool on);

void resetScreen();

// we may need to change this function to printSprite in accordance to the DXYN command
void printHexChar(uint8_t hexToPrint, int initialX, int initialY);

void printSprite(int initialX, int initialY, int rowCount);

void *drawThread(void *arg);
int updateWindow(struct mfb_window *window);