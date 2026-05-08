/* INDEXES:
* 0-1 = PC (Program Counter (for instructions))
 * 2-3 = I (Index Counter (for memory))
 * 4 = Delay Timer
 * 5 = Sound Timer
 * 6-21 = V0-VF GP registers
*/



#include "instructions.h"


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
    // jump, set PC to NNN


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

}

inline void op7()
{

}

inline void op8()
{

}

inline void op9()
{

}

inline void opA()
{

}

inline void opB()
{

}

inline void opC()
{

}

inline void opD()
{

}

inline void opE()
{

}

inline void opF()
{

}


