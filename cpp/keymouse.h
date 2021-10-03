#include "h/tools.h" 
#include "h/xtools.h"
#include "h/iotools.h" 
string appCodeName="keymouse";
string appTitle="keyMouse";
string configFileBaseName ="keymouse.conf";

bool printKeyMode=false;// true to enter Print key codes mode
pthread_t tid;
bool canExit = false;

Display *d , *dedicatedDpy= NULL; //display
Window xDefaultRootWin,rootWindow; //root Window



int speed_boost=200, speed_slow=2800;
int freq=speed_slow;
int pas = 1; //step move
bool verboz=true;

//Key states :
int vk_speed=XK_Alt_L; // 65513 Alt Key  for more speed
string controller_keys="65507,65515"; //default controller keys //Ctrl_L=65507  ctrl_r=65508//Meta key (windows key) = 65515
map<int,int>controllerKeys;
int vk_up= 65362;
int vk_left=  65361 ;
int vk_down= 65364;
int vk_right= 65363;
bool lockGrabKeys = false;
//Keys, for Mouse Buttons events
int vk_mouse_left=65508; //Ctrl_r(65508) or END(65507) default for convinient click, for right click, you might use thre dedicated button for that
bool vk_use_numpad=true; //ToDo, add to config
int vk_mouse_1 = 65457;  //Num_1=65457      //can use num pad 1, 2 , 3 to control mouse buttons
int vk_mouse_2=65458; //Num_2=65458
int vk_mouse_3=65459;  //Num_3=65459

map<int,short>keystates;

/*calcul de factoriel*/
long long fact(long long n)
{//cas particuliers: 0! et 1!
if((n==0)||(n==1)){return 1;}
return n*fact(n-1);
}

// // Utilisation :  get all combinations for modiders masks
//  vector<int> vlist={ ControlMask,ShiftMask,AltMask,NumLockMask};
//  auto combinations = getModifierComb(vlist);
//     for(auto m : combinations)
//     {
//         cout<<m.second<<endl;
//     }
map<int,int> getModifierComb(vector<int> vlist)
{
    map<int,int>results;
        
    long long n_count = fact(vlist.size() );
    //cout<<"ncount="<<n_count<<endl;
    int vmem=0; int ntour=0;
    int result=0, offset=0;
    for(int i=0; i<n_count; i++)
    {
        //cout<<"i="<<i<<endl;
        int m = i % vlist.size();
        int v= vlist[m];
        //cout<<"m="<<m<<" "<<"v="<<v<<endl;        
        if(m==0) //modulo Nouveau tour
        {        
            //cout<<"vmem+=v["<<ntour<<"] soit "<<vlist[ntour]<<endl;
            
            //cout<<"ntour="<<ntour<<" vmem="<<vmem<<endl;
            ntour++;
            //vmem|=offset; //vmem on previous offset
            offset = vlist[ntour-1];//get equivalent 
            if(ntour>=vlist.size() ) 
            {
                ntour=1;
                vmem|=vlist[ntour-1];
            }
        }         
        result= vmem|offset | v;
        results[result]=result;//one -> one
        //cout<<"RESULT="<<result<<endl;

    }//next

    //add all 
    result=0;
    for(auto v :vlist)
    {
        result|=v;
    }
    //cout<<"LAST RESULT="<<result<<endl;
    results[result]=result;    
    return results;
}




//vector<int> modifiersList={ ControlMask,ShiftMask,AltMask,NumLockMask,CapsLockMask,MetaMask};  //CapsLock and Meta not working
vector<int> modifiersList={ ControlMask,ShiftMask,AltMask,NumLockMask};
//grab key regardeless of modifiers
int xGrabIndependantKey(Display*d, int keycode, unsigned int window,string grab_or_ungrab="grab")
{    
    int r; bool silent = true;
    unsigned int    modifiers;           
    XUngrabKey(d, keycode, AnyModifier,window);    
    map<int,int> allModifiers = getModifierComb(modifiersList);
    allModifiers[0]=0;//Add none modifiers
    
    if(grab_or_ungrab=="grab")
    {
        if(!silent) cout<<"Grab the key "<<keycode<<endl;
        for(auto mm : allModifiers)
        {    
            modifiers = mm.second;
            r=XGrabKey(d, keycode, modifiers, window, True, GrabModeAsync, GrabModeAsync);
            if(!silent) cout<<"XGrabKey mod"<<modifiers<<"=>"<<r<<endl;
        }//next modifier
    }else if(grab_or_ungrab=="ungrab")
    {
         if(!silent) cout<<"UnGrab the key "<<keycode<<endl;
        for(auto mm : allModifiers)
        {    
            modifiers = mm.second;
            r=XUngrabKey(d, keycode, modifiers, window);
             if(!silent) cout<<"XUnGrabKey mod"<<modifiers<<"=>"<<r<<endl;
        }//next modifier
    }
   return r;
}//grab Independant Key


//Error Handling (vital) not prevent program exit.
int x_error_handler( Display* dpy, XErrorEvent* pErr )
{
    printf("X Error Handler called, values: %d/%lu/%d/%d/%d\n",
        pErr->type,
        pErr->serial,
        pErr->error_code,
        pErr->request_code,
        pErr->minor_code );
    if( pErr->request_code == 33 ){  // 33 (X_GrabKey)
        if( pErr->error_code == BadAccess ){
            printf("ERROR: A client attempts to grab a key/button combination already\n"
                   "        grabbed by another client. Ignoring.\n");
            return 0;
        }
    }
    //exit(1);  // exit the application for all unhandled errors.
    return 0;
}


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


bool keysGrabbed = false;
void grabKeys(string grab_or_ungrab="grab")
{
    //KeyCode kc=  XKeysymToKeycode(d,  XK_A);
    for(auto mKstate : keystates)
    {
        KeyCode kc=mKstate.first;
        xGrabIndependantKey(d, kc ,rootWindow, grab_or_ungrab);    
        usleep(2);
    }
    if(grab_or_ungrab=="grab")
        keysGrabbed=true;
    else 
        keysGrabbed=false;
}
void unGrabKeys()
{    
    grabKeys("ungrab");
}//unGrabKeys





//--------- Apply key states , keypressed=>1  keyreleased=>0  // global var is :  map<int,short>keystates;
void updateKeyState(int keycode, short state)
{    
    if(verboz)
    {
        // cout<<"mouseAction("<<keycode<<", state="<<state<<")"<<endl;
        // cout<<"count(keycode)="<<keystates.count(keycode)<<endl;
    }
    if(keystates.count(keycode)>0) //check if map contains the key "keycode"
    {
        int prevState= keystates[keycode];
        keystates[keycode]=state;
        //printf("key[%d] => prev state = %d, new state = %d \n",keycode,prevState,state);
    }


    if( checkcontrollerKeys() ==1 )
    {
        puts("All controller keys down !");
        lockGrabKeys=true;
        if( !keysGrabbed) grabKeys();  //only once if not already grabed
    }else
    {
        puts("All controller keys released!");
        lockGrabKeys=false;
        if( keysGrabbed) unGrabKeys(); //only once if not already ungrabbed
    }
}//updateKeyState

 
 //Simulate mouse actions
void mouseAction(int keycode, short state)
{    
    int changes =0;
    if(verboz)
    {
       // cout<<"mouseAction("<<keycode<<", state="<<state<<")"<<endl;
    }
    // cout<<"count(keycode)="<<keystates.count(keycode)<<endl;
    // if(keystates.count(keycode)>0) //check if map contains the key "keycode"
    // {
    //     int prevState= keystates[keycode];
    //     keystates[keycode]=state;
    //     printf("key[%d] => prev state = %d, new state = %d \n",keycode,prevState,state);
    // }

    Display * dpy=d;
    //Simulate Mouse Events :
    if(keycode == vk_mouse_left)  //Left Click from CTRL_r key
    { changes++;
        if(state==1)
        {
            mouseDown(1,dpy);        
        }
        if(state==0)
        {
            mouseUp(1,dpy);
        }
    }

    if(keycode == vk_mouse_1)  //Left click from Num Pad
    {changes++;
        if(state==1)
        {
            mouseDown(1,dpy);        
        }
        if(state==0)
        {
            mouseUp(1,dpy);
        }
    }

    if(keycode == vk_mouse_2)  //Right click from Num Pad
    {changes++;
        if(state==1)
        {
            mouseDown(2,dpy);        
        }
        if(state==0)
        {
            mouseUp(2,dpy);
        }
    }

    if(keycode == vk_mouse_3)  //Left click from Num Pad
    {changes++;
        if(state==1)
        {
            mouseDown(3,dpy);        
        }
        if(state==0)
        {
            mouseUp(3,dpy);
        }
    }

    if(changes>0)
    {
        cout<<"mouseActioned("<<keycode<<", state="<<state<<")"<<endl;
    }
}//mouseActions



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


    //Catch X errors! 
    XSetErrorHandler( x_error_handler );

}//initApp


void unInitApp(string from="main")
{
    canExit=true;    
    
    if(from!="thread")
    {          
        usleep(freq);    
        usleep(10);
    }else
    {       
    }
    if(dedicatedDpy!=NULL){ XCloseDisplay(dedicatedDpy); dedicatedDpy=NULL;}
    if(d!=NULL) {XCloseDisplay(d); d=NULL;}    
}




//Thread Job, loop
void *_keyStateLoop(void * arg)
{
    int mx,my;
    short changes = 0;

    while(!canExit)    
    {
        //puts("In loop");
        if (keysGrabbed)
        {
            // //retrieve mouse position          
            getMousePos(mx, my, dedicatedDpy,xDefaultRootWin);
            printf("MousePos is (%d,%d)      \r", mx, my);
            fflush(stdout);
            //Change speed :
            if( keystates[vk_speed]==1){
                freq=speed_boost;
            }else{ freq = speed_slow;}

            changes = 0;
            //Change mouse mosition (Move mouse) :
            if (keystates[vk_up] == 1)
            {
                my -= pas;
                changes++;
            }
            if (keystates[vk_down] == 1)
            {
                my += pas;
                changes++;
            }
            if (keystates[vk_left] == 1)
            {
                mx -= pas;
                changes++;
            }
            if (keystates[vk_right] == 1)
            {
                mx += pas;
                changes++;
            }

            if (changes > 0)
            {
                cout<<"---CHANGE POZ-----"<<changes<<"   ";
                //setMousePos(mx,my,d,rootWindow);
                setMousePos(mx,my,dedicatedDpy,xDefaultRootWin);
            }         

        } //do things only if in grabbed state

        usleep(2+freq); //Always sleep
    }//wend

    cout<<"Exiting thread."<<endl;
    unInitApp("thread"); ///closes displays

    return NULL;
}//threadJob