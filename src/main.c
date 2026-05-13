

#include "display.h"
#include "memory.h"
#include "processor.h"

//#include <pthread.h>

#include "system.h"




int main(void)
{
    initializeFont();

    loadProgram("../test_programs/ibm.ch8");

    pthread_t processThread;
    if (pthread_create(&processThread, NULL, FDE, NULL) != 0)
    {
        printf("%s", "Couldn't create background logic thread.");
        return 1;
    };


    struct mfb_window *window = mfb_open_ex("my display", WIDTH, HEIGHT, MFB_WF_RESIZABLE);

    pthread_t draw_tid;
    pthread_create(&draw_tid, NULL, drawThread, NULL);

    updateWindow(window);

    pthread_join(draw_tid, NULL);
    mfb_close(window);


    pthread_join(processThread, NULL);

    return 0;
}
