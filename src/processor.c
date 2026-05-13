#include "processor.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "system.h"
#include "display.h"


uint8_t op;
uint8_t X;
uint8_t Y;
uint8_t N;

uint8_t NN;
uint16_t NNN;


static inline void op0() // this works!
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

static inline void op1()
{
    // jump: set PC to NNN
    cpuRegisters.programCounter = NNN;
}

static inline void op2()
{

}

static inline void op3()
{

}

static inline void op4()
{

}

static inline void op5()
{

}

static inline void op6()
{
    // 6XNN (set register VX) THIS WORKS!
    cpuRegisters.V[X] = NN;
}

static inline void op7()
{
    //7XNN (add value to register VX)
    cpuRegisters.V[X] += NN;
}

static inline void op8()
{

}

static inline void op9()
{

}

static inline void opA()
{
    //ANNN (set index register I)
    cpuRegisters.indexPointer = NNN;
    // this works!
}

static inline void opB()
{

}

static inline void opC()
{

}

static inline void opD()
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

    // The starting position will wrap
    // but the drawing of the sprite itself will not.

    // ensure that the initial coords wrap around the bounds and call the printSprite function

    printSprite(cpuRegisters.V[X] % WIDTH, cpuRegisters.V[Y] % HEIGHT, N);

    // for testing purposes..
    //printHexChar(0xC, 4, 4);
}

static inline void opE()
{

}

static inline void opF()
{

}



void instruction_execute(const uint16_t instruction)
{
    op = ((instruction & 0b1111000000000000) >> 12); // using this for the switch statement
    X = ((instruction & 0b0000111100000000) >> 8); // register
    Y = ((instruction & 0b0000000011110000) >> 4); // register
    N = (instruction & 0b0000000000001111); // 4 bit number

    NN = ((Y << 4) | N); // 8-bit IMMediate number
    NNN = ((X << 8) | NN); // 12-bit address

    /* start with:
    00E0 (clear screen) [easy]
    1NNN (jump) [set PC to NNN=
    6XNN (set register VX)
    7XNN (add value to register VX)
    ANNN (set index register I)
    DXYN (display/draw)
    */

    //printf("%x\n", op);
    //printf("%x\n", NNN);

    if (op == 0x0) {op0();}
    else if (op == 0x1) {op1();}
    else if (op == 0x6) {op6();}
    else if (op == 0x7) {op7();}
    else if (op == 0xA) {opA();}
    else if (op == 0xD) {opD();}
    else {printf("%s\n", "INVALID OPCODE");}

}

// TODO: more robust checking





/* INDEXES:
 * 0-1 = PC (Program Counter (for instructions))
 * 2-3 = I (Index Counter (for memory))
 * 4 = Delay Timer
 * 5 = Sound Timer
 * 6-21 = V0-VF GP registers
*/




uint16_t fetch()
{
    // read two-byte instruction from PC
    const uint16_t PC = cpuRegisters.programCounter;


    // increment PC by 2
    cpuRegisters.programCounter = PC + 2;

    // we're fetching the MEMORY FROM the address, NOT the address itself
    return (systemMemory[PC] << 8) | systemMemory[(PC + 1)];
}


void *FDE(void *arg)
{
    printf("%s\n", "Starting FDE cycle:");
    cpuRegisters.programCounter = 0x0200; // to ensure that the thing gets loaded correctly

    while (atomic_load(&running)) {

        instruction_execute(fetch());

        usleep(1435.0f); // TODO: another magic number to get rid of,
        // this gets us to about 700 instructions per second
    }
}