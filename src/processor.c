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
        if (NN == 0xE0) { resetScreen(); } //00E0
        else if (NN == 0xEE) {cpuRegisters.programCounter = stackPop(); } // 00EE
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

// 3XNN will skip one instruction if the value in VX is equal to NN...
static inline void op3()
{
    if (cpuRegisters.V[X] == NN)
    {
        cpuRegisters.programCounter += 2;
    }
}

// ... 4XNN will skip if they are not equal.
static inline void op4()
{
    if (cpuRegisters.V[X] != NN)
    {
        cpuRegisters.programCounter += 2;
    }
}

// 5XY0 skips if the values in VX and VY are equal
static inline void op5()
{
    if (cpuRegisters.V[X] == cpuRegisters.V[Y])
    {
        cpuRegisters.programCounter += 2;
    }
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
    switch (N)
    {
    case 0: cpuRegisters.V[X] = cpuRegisters.V[Y]; break; // Set (VX = VY)
    case 1: cpuRegisters.V[X] = cpuRegisters.V[X] | cpuRegisters.V[Y]; cpuRegisters.V[0xF] = 0; break; // Bitwise OR (VX | VY)
    case 2: cpuRegisters.V[X] = cpuRegisters.V[X] & cpuRegisters.V[Y]; cpuRegisters.V[0xF] = 0; break;// Bitwise AND (VX & VY)
    case 3: cpuRegisters.V[X] = cpuRegisters.V[X] ^ cpuRegisters.V[Y]; cpuRegisters.V[0xF] = 0; break;// Bitwise XOR (VX ^ VY)
    case 4: ;
        int actualResult;
        actualResult = cpuRegisters.V[X] + cpuRegisters.V[Y];
        cpuRegisters.V[X] = cpuRegisters.V[X] + cpuRegisters.V[Y];
        if (actualResult > 255) {cpuRegisters.V[0xF] = 1;} else {cpuRegisters.V[0xF] = 0;}
        break;// Add (VX = VX + VY)
    case 5: {
        const uint8_t oldVX = cpuRegisters.V[X]; // TODO: reign the scope of this variable in
        cpuRegisters.V[X] = cpuRegisters.V[X] - cpuRegisters.V[Y];
        if (oldVX >= cpuRegisters.V[Y]) {cpuRegisters.V[0xF] = 1;} else {cpuRegisters.V[0xF] = 0;}

        break;// Subtract 1 (VX = VX - VY) [carry flag]
    }

    case 6:
    {
        cpuRegisters.V[X] = cpuRegisters.V[Y];
        const uint8_t oldVX = cpuRegisters.V[X];
        cpuRegisters.V[X] = cpuRegisters.V[X] >> 1; // Shift right
        if ((oldVX & 0b00000001) == 1) {cpuRegisters.V[0xF] = 1;} else {cpuRegisters.V[0xF] = 0;}

        // TODO: (maybe) configure cases 6 and e to do the ambiguous part)
        break;
    }

    case 7:
        cpuRegisters.V[X] = cpuRegisters.V[Y] - cpuRegisters.V[X];

        if (cpuRegisters.V[Y] >= cpuRegisters.V[X]) {cpuRegisters.V[0xF] = 1;} else {cpuRegisters.V[0xF] = 0;}

        break;// Subtract 2 (VX = VY - VX) [carry flag]
    case 0xE:
    {
        cpuRegisters.V[X] = cpuRegisters.V[Y];
        const uint8_t oldVX = cpuRegisters.V[X];
        cpuRegisters.V[X] = cpuRegisters.V[X] << 1; // Shift left
        if ((oldVX & 0b10000000) >> 7 == 1) {cpuRegisters.V[0xF] = 1;} else {cpuRegisters.V[0xF] = 0;}
        break;
    }

    default:
        break;
    }
}

//9XY0 skips if the values in VX and VY are not equal
static inline void op9()
{
    if (cpuRegisters.V[X] != cpuRegisters.V[Y])
    {
        cpuRegisters.programCounter += 2;
    }
}

static inline void opA() { cpuRegisters.indexPointer = NNN; } // ANNN

static inline void opB() // TODO: another ambiguous instruction, tie this into the other one
{
    cpuRegisters.programCounter = NNN + cpuRegisters.V[0];
    // or later on...
    //cpuRegisters.programCounter = NNN + cpuRegisters.V[X];
}

static inline void opC()
{
    // generate random number, bitwise ANDs it with NN, put result in VX

    srand(time(NULL));// seed ONLY ONCE with the current time (TODO: bring this closer to the program's beginning)
    const uint8_t randomNum = rand() % 0xFF;
    cpuRegisters.V[X] = randomNum & NN;

}

//The first thing to do is to get the X and Y coordinates from VX and VY.

// The starting position will wrap
// but the drawing of the sprite itself will not.

// ensure that the initial coords wrap around the bounds and call the printSprite function
static inline void opD() { printSprite(cpuRegisters.V[X] % LOGICAL_WIDTH, cpuRegisters.V[Y] % LOGICAL_HEIGHT, N); }

static inline void opE()
{
    // finally time for keyboard input...
    /*1 2 3 c
     *4 5 6 d
     *7 8 9 e
     *a 0 b f
     *equals...
     *1 2 3 4
     *q w e r
     *a s d f
     *z x c v
     */

    // for now, we will just assume that everybody uses QWERTY and implement scancodes a bit later. We must build an MVP
    // TODO: scancodes

    // abort if V[X] isn't a value between 0x0 and 0xF
    if (cpuRegisters.V[X] > 0xF)
    {
        // TODO: do something here to abort
    }

    // extract bit, turn bit into bool
    bool isKeyPressed = ((keystate >> cpuRegisters.V[X]) & 1) != 0;


    //EX9E: PC += 2 if key V[X] is being held down currently
    if (Y == 9) {if (isKeyPressed) {cpuRegisters.programCounter += 2;}}

    //EXA1: PC += 2 if key V[X] is NOT being held down currently
    if (Y == 0xA) {{if (!isKeyPressed) {cpuRegisters.programCounter += 2;}}}

}

static inline void opF()
{
    switch (NN)
    {
    case 0x07:
        cpuRegisters.V[X] = cpuRegisters.delayTimer;
        break;
    case 0x15:
        cpuRegisters.delayTimer = cpuRegisters.V[X];
        break;
    case 0x18:
        cpuRegisters.soundTimer = cpuRegisters.V[X];
        break;
    case 0x1e:
        cpuRegisters.indexPointer += cpuRegisters.V[X];
        break;
    case 0x0a:
        bool isKeyPressed = false;
        while (!isKeyPressed)
        {
            isKeyPressed = ((keystate >> cpuRegisters.V[X]) & 1) != 0;
        }
        // TODO: maybe do some PC manipulation here

        break;
    case 0x29:
        cpuRegisters.indexPointer = (0x50 + (cpuRegisters.V[X] * 0x5));

        break;
    case 0x33: ;
        int theNumber;
        theNumber = cpuRegisters.V[X];
        systemMemory[cpuRegisters.indexPointer] = theNumber / 100;
        theNumber %= 100;
        systemMemory[cpuRegisters.indexPointer + 1] = theNumber / 10;
        theNumber %= 10;
        systemMemory[cpuRegisters.indexPointer + 2] = theNumber;
        break;
    case 0x55:
        for (int i = 0; i <= X; i++)
        {
            systemMemory[cpuRegisters.indexPointer + i] = cpuRegisters.V[i];
        }
        cpuRegisters.indexPointer += X + 1;
        // TODO: make this configurable since it's an ambiguous instruction
        break;
    case 0x65:
        for (int i = 0; i <= X; i++)
        {
            cpuRegisters.V[i] = systemMemory[cpuRegisters.indexPointer + i];
        }
        cpuRegisters.indexPointer += X + 1;
        break;

    default:
        break;
    }
}



void instruction_execute(const uint16_t instruction)
{
    op = (instruction & 0b1111000000000000) >> 12; // using this for the switch statement
    X = (instruction & 0b0000111100000000) >> 8; // register
    Y = (instruction & 0b0000000011110000) >> 4; // register
    N = instruction & 0b0000000000001111; // 4 bit number

    NN = instruction & 0b0000000011111111; // 8-bit IMMediate number
    NNN = instruction & 0b0000111111111111; // 12-bit address

    switch (op)
    {
    case 0x0: op0(); break;
    case 0x1: op1(); break;
    case 0x2: op2(); break;
    case 0x3: op3(); break;
    case 0x4: op4(); break;
    case 0x5: op5(); break;
    case 0x6: op6(); break;
    case 0x7: op7(); break;
    case 0x8: op8(); break;
    case 0x9: op9(); break;
    case 0xA: opA(); break;
    case 0xB: opB(); break;
    case 0xC: opC(); break;
    case 0xD: opD(); break;
    case 0xE: opE(); break;
    case 0xF: opF(); break;
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

        //usleep(15000); // slowdown for debugging

        //usleep(1429.0f); // TODO: another magic number to get rid of,
        //
        // this gets us to about 700 instructions per second.

        // every 11.666 times, we will need to increment a counter... is there a better way?
        // yes, there is. new thread, just a timer thread, does *something* exactly 60 times a second.
        // OR... already use the display's 60fps limit to our advantage. One frame is one tick for the timers.

    }
    return 0;
}