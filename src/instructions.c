
#include "instructions.h"

#include <display.h>
#include "system.h"



uint8_t op;
uint8_t X;
uint8_t Y;
uint8_t N;

uint8_t NN;
uint16_t NNN;
void instruction_execute(const uint16_t instruction)
{
    op = ((instruction & 0b1111000000000000) >> 12); // using this for the switch statement
    X = ((instruction & 0b0000111100000000) >> 8); // register
    Y = ((instruction & 0b0000000011110000) >> 4); // register
    N = (instruction & 0b0000000000001111); // 4 bit number

    NN = ((Y << 4) | N); // 8-bit IMMediate number
    NNN = ((X << 8) | X); // 12-bit address

    /* start with:
    00E0 (clear screen) [easy]
    1NNN (jump) [set PC to NNN=
    6XNN (set register VX)
    7XNN (add value to register VX)
    ANNN (set index register I)
    DXYN (display/draw)
    */

}

// TODO: more robust checking

inline void op0()
{
    if (X == 0)
    {
        // verify that NNN = OE0
        resetScreen();
    } else
    {
        // SOME sort of error handling
        printf("invalid");
    }
}

inline void op1()
{
    // jump: set PC to NNN
    cpuRegisters.programCounter = NNN;
}

inline void op2()
{

}

inline void op3()
{

}

inline void op4()
{

}

inline void op5()
{

}

inline void op6()
{
    // 6XNN (set register VX)
    cpuRegisters.V[X] = NN;

}

inline void op7()
{
    //7XNN (add value to register VX)
    cpuRegisters.V[X] += NN;
}

inline void op8()
{

}

inline void op9()
{

}

inline void opA()
{
    //ANNN (set index register I)
    cpuRegisters.indexPointer = NNN;

}

inline void opB()
{

}

inline void opC()
{

}

inline void opD()
{
    //DXYN (display/draw)

    /*
    It will draw an N pixels tall sprite from the memory location that the I index register is
    holding to the screen, at the horizontal X coordinate in VX and the Y coordinate in VY. All
    the pixels that are “on” in the sprite will flip the pixels on the screen that it is drawn to
    (from left to right, from most to least significant bit). If any pixels on the screen were turned
    “off” by this, the VF flag register is set to 1. Otherwise, it’s set to 0.
    */

    // oh lord...
    //The first thing to do is to get the X and Y coordinates from VX and VY.
    uint8_t vx_coord = cpuRegisters.V[X];
    uint8_t vy_coord = cpuRegisters.V[Y];

    // The starting position will wrap
    // but the drawing of the sprite itself will not.

    // ensure that the initial coords are in bounds:
    vx_coord = vx_coord % WIDTH;
    vy_coord = vx_coord % HEIGHT;



}

inline void opE()
{

}

inline void opF()
{

}


