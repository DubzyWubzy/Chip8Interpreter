#include <stdio.h>

#include "processor.h"


uint8_t cpuRegisters[22];

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
    const uint16_t PC = {(cpuRegisters[0] << 8) | cpuRegisters[1]};

    // you're fetching the MEMORY FROM the address, NOT the address itself

    // increment PC by 2
    cpuRegisters[0] = (PC + 2) >> 8;
    cpuRegisters[1] = (PC + 2);


    return (systemMemory[PC] << 8) | (systemMemory[PC + 1]);
}

void decodeAndExecute(uint16_t instruction)
{


}

void FDE()
{
    cpuRegisters[0] = 0x02; // so that the program space starts at PC = 0x0200
    cpuRegisters[1] = 0x00; // TODO: put this in some sort of initCpu function

    while (true) {

        decodeAndExecute(fetch());
        // this gets us to about 700 instructions per second
        usleep(10000.0f/7.0f); // TODO: another magic number to get rid of
    }
}