

#include "display.h"
#include "memory.h"
#include "processor.h"

#include <pthread.h>

uint32_t buffer[WIDTH * HEIGHT]; // this is the main GUI window buffer
uint8_t systemMemory[4096]; // TODO: initialize this in its own function
uint8_t cpuRegisters[22];


void* backgroundActivities(void* arg)
{
    FDE(systemMemory, cpuRegisters);
}


int main(void)
{
    pthread_t processThread;
    if (pthread_create(&processThread, NULL, backgroundActivities, NULL) != 0)
    {
        printf("%s", "Couldn't create background non-UI thread.");
        return 1;
    };
    initializeFont(systemMemory);

    printHexChar(systemMemory, 0xF, 30, 10, buffer);

    drawWindow(systemMemory, buffer);




}
