#include "h/tools.h" 
#include "h/xtools.h"
#include "h/iotools.h" 
string appCodeName="keymouse";
string appTitle="keyMouse";
string configFileBaseName ="keymouse.conf";

bool printKeyMode=false;// true to enter Print key codes mode
pthread_t tid;
bool canExit = false;

Display *d = NULL; //display
Window rootWindow; //root Window



int speed_boost=200, speed_slow=6200;
int freq=speed_slow;
 

//Key states :
int vk_speed=XK_Alt_L; // 65513 Alt Key  for more speed
string controller_keys="65507,65515"; //default controller keys //Ctrl_L=65507  ctrl_r=65508//Meta key (windows key) = 65515
map<int,int>controllerKeys;
int vk_up= 65362;
int vk_left=  65361 ;
int vk_down= 65364;
int vk_right= 65363;
bool lockGrabKeys = false;
// int vk_mouse_left=Key.ctrl_r //Ctrl_r default for click, for right click, might use thre dedicated button
// bool vk_use_numpad=true ; //ToDo, add to config
// int vk_mouse_1 =   //Num_1=65457      //can use num pad 1, 2 , 3 to control mouse buttons
// int vk_mouse_2='2' //Num_2=65458
// int vk_mouse_3='3'  //Num_3=65459


//--------- Apply key states , keypressed=>1  keyreleased=>0
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


//grab key regardeless of modifiers
int xGrabIndependantKey(Display*d, int keycode, unsigned int window)
{
    cout<<"Grab the key "<<keycode<<endl;
    int r;
    unsigned int    modifiers;
    
    // modifiers = NumLockMask; //ControlMask | ShiftMask | Mod2Mask;    
    // r=XGrabKey(d, keycode, modifiers, window, True, GrabModeAsync, GrabModeAsync);
    // cout<<"XGrabKey + NumLockMask mod=>"<<r<<endl;

     modifiers =   MetaMask ; //ControlMask | ShiftMask | Mod2Mask;    
    // r=XGrabKey(d, keycode, modifiers, window, True, GrabModeAsync, GrabModeAsync);
    // cout<<"XGrabKey + MetaMask  mod=>"<<r<<endl;

    XUngrabKey(d, keycode, AnyModifier,window);

    modifiers =  AltMask ;
    r=XGrabKey(d, keycode, AnyModifier, window, True, GrabModeAsync, GrabModeAsync);
    cout<<"XGrabKey + AltMask  mod=>"<<r<<endl;  
    // r=XGrabKey(d, keycode, modifiers, window, True, GrabModeAsync, GrabModeAsync);
    // cout<<"XGrabKey + ControlMask mod=>"<<r<<endl;

    // modifiers=0;
    // r=XGrabKey(d, keycode, modifiers, window, True, GrabModeAsync, GrabModeAsync);
    // cout<<"XGrabKey 0 mod=>"<<r<<endl;
   return r;
}//grab Independant Key




bool keysGrabbed = false;
void grabKeys()
{
    //KeyCode kc=  XKeysymToKeycode(d,  XK_A);
    KeyCode kc=XKC_UP;
    xGrabIndependantKey(d, kc ,rootWindow);
    printf("key is grabbed (%d) \n",kc );
    sleep(1);
    keysGrabbed=true;
}
void unGrabKeys()
{
    //XUngrabKey(d,keycode,modifiers,root);
    keysGrabbed=false;
}//unGrabKeys



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


    // //Fill map with controller keys
    for (size_t i = 0; i < controllerKeys.size(); i++)
    {
        int kcode = controllerKeys[i];
        cout<<"Filling keystates["<<kcode<<"] with -1 "<<endl;
        keystates[kcode]=-1;
    }
    // add fill speed key    
    keystates[vk_speed]=-1;
    // add directionnal keys
    keystates[vk_up]=-1;
    keystates[vk_left]=-1;
    keystates[vk_down]=-1;
    keystates[vk_right]=-1;

    freq=speed_slow; //default freq is speed slow   
}//initApp



//checks if all controller keys are down
short checkcontrollerKeys()
{ 
    int count_down=0;
      // //Fill map with controller keys
    for (size_t i = 0; i < controllerKeys.size(); i++)
    {
        int kcode = controllerKeys[i];         
        if(keystates[kcode]==1){ count_down++;}
        //cout << count_down <<endl;
    }
    if( count_down >= controllerKeys.size() )
    {
        return 1;
    }
    return 0;
}



//Thread Job
void *_keyStateLoop(void * arg)
{

    while(!canExit)
    {
       // puts("in loop");

        //check controller keys all down
        if( checkcontrollerKeys() ==1 )
        {
            //puts("All controller keys down !");
            lockGrabKeys=true;
            if( !keysGrabbed) grabKeys();
        }else
        {
            lockGrabKeys=false;
            if( keysGrabbed) unGrabKeys();
        }
        
        usleep(2+freq);
    }//wend



    return NULL;
}//threadJob