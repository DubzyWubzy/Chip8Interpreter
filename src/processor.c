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
    case 1: cpuRegisters.V[X] = cpuRegisters.V[X] | cpuRegisters.V[Y]; break; // Bitwise OR (VX | VY)
    case 2: cpuRegisters.V[X] = cpuRegisters.V[X] & cpuRegisters.V[Y]; break;// Bitwise AND (VX & VY)
    case 3: cpuRegisters.V[X] = cpuRegisters.V[X] ^ cpuRegisters.V[Y]; break;// Bitwise XOR (VX ^ VY)
    case 4: cpuRegisters.V[X] = cpuRegisters.V[X] + cpuRegisters.V[Y]; break;// Add (VX = VX + VY)
    case 5: cpuRegisters.V[X] = cpuRegisters.V[X] - cpuRegisters.V[Y]; break;// Subtract 1 (VX = VX - VY) [carry flag]
    case 6:
        cpuRegisters.V[0xF] = (cpuRegisters.V[X] & 0b00000001); // TODO: (maybe) make this a more efficient data type?
        cpuRegisters.V[X] = cpuRegisters.V[X] >> 1; // Shift right
        // TODO: (maybe) configure cases 6 and e to do the ambiguous part)
        break;
    case 7: cpuRegisters.V[X] = cpuRegisters.V[Y] - cpuRegisters.V[X]; break;// Subtract 2 (VX = VY - VX) [carry flag]
    case 0xE:
        cpuRegisters.V[0xF] = (cpuRegisters.V[X] & 0b10000000) >> 7;
        cpuRegisters.V[X] = cpuRegisters.V[X] << 1; // Shift left
        break;
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
    cpuRegisters.programCounter += cpuRegisters.V[0];
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
static inline void opD() { printSprite(cpuRegisters.V[X] % WIDTH, cpuRegisters.V[Y] % HEIGHT, N); }

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
    if (Y == 0xA) {{if (~isKeyPressed) {cpuRegisters.programCounter += 2;}}}

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
        //uint16_t ogIndex = cpuRegisters.indexPointer;
        int theNumber;
        theNumber = cpuRegisters.V[X];
        systemMemory[cpuRegisters.indexPointer] = theNumber / 100;
        theNumber %= 100;
        systemMemory[cpuRegisters.indexPointer + 1] = theNumber / 10;
        theNumber %= 10;
        systemMemory[cpuRegisters.indexPointer + 2] = theNumber;
        break;
    case 0x55:

        for (int i = 0; i == X; i++)
        {
            systemMemory[cpuRegisters.indexPointer + i] = cpuRegisters.V[i];
        }
        break;
    case 0x65:
        for (int i = 0; i == X; i++)
        {
            cpuRegisters.V[i] = systemMemory[cpuRegisters.indexPointer + i];
        }
        break;

    default:
        break;
    }
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

        usleep(150000); // for debugging

        //usleep(1429.0f); // TODO: another magic number to get rid of,
        //
        // this gets us to about 700 instructions per second.

        // every 11.666 times, we will need to increment a counter... is there a better way?
        // yes, there is. new thread, just a timer thread, does *something* exactly 60 times a second.
        // OR... already use the display's 60fps limit to our advantage. One frame is one tick for the timers.

    }
    return 0;
}