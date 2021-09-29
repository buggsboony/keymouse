#include "h/tools.h" 
#include "h/xtools.h"
#include "h/iotools.h"

string appCodeName="keymouse";
string appTitle="keyMouse";
string configFileBaseName ="keymouse.conf";

pthread_t tid;
bool canExit = false;


int speed_boost=200, speed_slow=6200;
int freq=speed_slow;
 
void initApp()
{
    freq=speed_slow; //default freq is speed slow

}//initApp


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