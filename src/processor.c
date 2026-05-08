#include "processor.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <processor.h>
#include "system.h"



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
    return systemMemory[PC];
}

void decodeAndExecute(uint16_t instruction)
{


}

void FDE()
{
    cpuRegisters.programCounter = 0x0002;

    while (true) {

        decodeAndExecute(fetch());
        // this gets us to about 700 instructions per second
        usleep(10000.0f/7.0f); // TODO: another magic number to get rid of
    }
}