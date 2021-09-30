#include "h/tools.h" 
#include "h/xtools.h"
#include "h/iotools.h"

string appCodeName="keymouse";
string appTitle="keyMouse";
string configFileBaseName ="keymouse.conf";

bool printKeyMode=false;// true to enter Print key codes mode
pthread_t tid;
bool canExit = false;


int speed_boost=200, speed_slow=6200;
int freq=speed_slow;
 

//Key states :

//pour keys here
int vk2=XK_Control_L; //Ctrl_L=65507  ctrl_r=65508
int vk1=XK_Super_L;  //Meta key (windows key) = 65515
int vk_speed=XK_Alt_L; // 65513 Alt Key  for more speed
string controller_keys="65507,65515"; //default controller keys
map<int,int>controllerKeys;
// int vk_up=Key.up 65362
// int vk_left=Key.left  65361 
// int vk_down=Key.down 65364
// int vk_right=Key.right  65363

// int vk_mouse_left=Key.ctrl_r //Ctrl_r default for click, for right click, might use thre dedicated button
// bool vk_use_numpad=true ; //ToDo, add to config
// int vk_mouse_1 =   //Num_1=65457      //can use num pad 1, 2 , 3 to control mouse buttons
// int vk_mouse_2='2' //Num_2=65458
// int vk_mouse_3='3'  //Num_3=65459

map<int,short>keystates;

void updateKeyState(int keycode, short state)
{    
    cout<<"updateKeyState("<<keycode<<")"<<endl;
    cout<<"count(keycode)="<<keystates.count(keycode)<<endl;
    if(keystates.count(keycode)>0) //check if map contains the key "keycode"
    {
        int prevState= keystates[keycode];
        keystates[keycode]=state;
        printf("key[%d] => prev state = %d, new state = %d \n",keycode,prevState,state);
    }
}//updateKeyState


//Init function
void initApp()
{
    //retreive list of controller keys 
    vector <string> slist = splitStr(controller_keys,',');
    for (size_t i = 0; i < slist.size(); i++)
    {
        int kcode = strToInt( trimStr(slist[i]) );
        cout<<"Adding kcode:"<<kcode<<endl;
        controllerKeys[controllerKeys.size()]=kcode;        
    }//next string keycode


    // //Fill map 
    for (size_t i = 0; i < controllerKeys.size(); i++)
    {
        int kcode = controllerKeys[i];
        cout<<"Filling keystates["<<kcode<<"] with -1 "<<endl;
        keystates[kcode]=-1;
    }
     
 

    freq=speed_slow; //default freq is speed slow   
}//initApp

//Thread Job
void *_keyStateLoop(void * arg)
{

    while(!canExit)
    {
       // puts("in loop");
        usleep(2+freq);
    }//wend



    return NULL;
}//threadJob