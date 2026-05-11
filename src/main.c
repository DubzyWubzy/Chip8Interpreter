

#include "display.h"
#include "memory.h"
#include "processor.h"

#include <pthread.h>

#include "system.h"


void* backgroundActivities(void* arg)
{
    //printHexChar(0xF, 30, 10);
    FDE();
    return 0;
}


int main(void)
{
    pthread_t processThread;
    if (pthread_create(&processThread, NULL, backgroundActivities, NULL) != 0)
    {
        printf("%s", "Couldn't create background non-UI thread.");
        return 1;
    };


    initializeFont();

    loadProgram("../test_programs/ibm.ch8");

    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, MFB_WF_RESIZABLE);

    printHexChar(0xF, 30, 10);

    //updateWindow(window);


    //pthread_join(processThread, NULL);

    window = NULL;
    return 0;
}
