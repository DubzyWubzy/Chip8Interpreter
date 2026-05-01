

#include "display.h"
#include "memory.h"
#include "processor.h"

#include <pthread.h>

void* backgroundActivities(void* arg)
{
    FDE(systemMemory);
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

    drawWindow(systemMemory);

}
