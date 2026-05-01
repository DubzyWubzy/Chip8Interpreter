#include <unistd.h>

#include "display.h"
#include "memory.h"
#include "processor.h"


int main(void)
{
    initializeFont(systemMemory);

    drawWindow(systemMemory); // TODO: put this on a separate thread

    // gonna put the fetch/decode/execute stuff in main for now and extract it later
    while (true) {
        // this gets us to about 700 instructions per second (hopefully)
        usleep(10000.0f/7.0f); // TODO: another magic number to get rid of
        
    }
}
