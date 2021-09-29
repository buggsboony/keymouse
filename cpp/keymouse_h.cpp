#include "h/tools.h" 


pthread_t tid;
bool canExit = false;

int freq_min=200, freq_max=6200;
int freq=2000;


//Thread Job
void *_keyStateLoop(void * arg)
{

    while(!canExit)
    {
        puts("in loop");
        usleep(2+freq);
    }//wend



    return NULL;
}//threadJob