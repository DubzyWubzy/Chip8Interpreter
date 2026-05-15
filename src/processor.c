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
    if (X == 0x0)
    {
        // verify that NNN = OE0
        if (N == 0x0) { resetScreen(); } //00E0
        else if (N == 0xE) {cpuRegisters.programCounter =  stackPop(); } // 00EE
        // TODO: test stack
    } else
    {
        // SOME sort of error handling
        printf("invalid");
    }
}

static inline void op1()
{
    // jump: set PC to NNN
    cpuRegisters.programCounter = NNN; // 1NNN
}

static inline void op2()
{
    stackPush(cpuRegisters.programCounter); // TODO: test stack
    cpuRegisters.programCounter = NNN; // 2NNN
}

// 3XNN will skip one instruction if the value in VX is equal to NN, and 4XNN will skip if they are not equal.
static inline void op3()
{

}

static inline void op4()
{

}

// 5XY0 skips if the values in VX and VY are equal, while 9XY0 skips if they are not equal.
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

static inline void opA() { cpuRegisters.indexPointer = NNN; } // ANNN

static inline void opB()
{

}

static inline void opC()
{

}

//The first thing to do is to get the X and Y coordinates from VX and VY.

// The starting position will wrap
// but the drawing of the sprite itself will not.

// ensure that the initial coords wrap around the bounds and call the printSprite function
static inline void opD() { printSprite(cpuRegisters.V[X] % WIDTH, cpuRegisters.V[Y] % HEIGHT, N); }

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

    switch (op)
    {
    case 0x0:
        op0(); break;
    case 0x1:
        op1(); break;
    case 0x6:
        op6(); break;
    case 0x7:
        op7(); break;
    case 0xA:
        opA(); break;
    case 0xD:
        opD(); break;
    default:
        printf("%s\n", "INVALID OPCODE");
        break;
    }

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
    return 0;
}