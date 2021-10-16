#include "h/tools.h" 
#include "h/xtools.h"
#include "h/iotools.h" 
#include <X11/cursorfont.h>
string appCodeName="keymouse", version="v1.3.113"; //vX.d.11m
string appTitle="keyMouse";
string configFileBaseName ="keymouse.conf";

bool printKeyMode=false, testClick=false, openConfig=false,resetConfig=false;// true to enter Print key codes mode
pthread_t tid;
bool canExit = false, softExit=false;

Display *d , *dedicatedDpy= NULL; //display
Window xDefaultRootWin,rootWindow; //root Window

int xc_cursor = XC_bogosity;
int xc_default =  XC_hand2;

bool modeTOR = true; //mode Tout Ou Rien
int subsituteCursor = 0; //cursor while locked
int speed_boost=200, speed_slow=2800;
int freq=speed_slow;
int pas = 1; //step move
bool verboz=false;
short verbozVal=(short)verboz;

//Key states :
//string controller_keys_TOR="65507,65515,65513"; //Controller keys TOR :    Ctrl+ Win + Alt
string controller_keys_TOR="60,65513"; //
int vk_speed_TOR=65505; // Shift
string controller_keys="65507,65515"; //default controller keys //Ctrl_L=65507  ctrl_r=65508//Meta key (windows key) = 65515
int vk_speed=XK_Alt_L; // 65513 Alt Key  for more speed
map<int,int>controllerKeys;
int vk_up= 65362;
int vk_left=  65361 ;
int vk_down= 65364;
int vk_right= 65363;
bool lockGrabKeys = false;
//Keys, for Mouse Buttons events
int vk_mouse_left=65508; //vk_mouse_left Ctrl_r(65508) or END(65367) default for convinient click, for right click, you might use thre dedicated button for that
int vk_alternative_mouse_left=65367;
bool vk_use_numpad=true; //ToDo, add to config
int vk_mouse_1 = 65457;  //vk_mouse_1 Num_1=65457      //can use num pad 1, 2 , 3 to control mouse buttons
int vk_mouse_2=65458;    //vk_mouse_2  Num_2=65458   (Middle Click
int vk_mouse_3=65459;    //vk_mouse_3 Num_3=65459   (Right Click)  
int vk_mouse_right=65435; //65366 PageDown or 65435: Num_3 without Numlock activated!

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
    if(verboz)
    {
        cout<<"chkControlKeys:";
    }
      // //Fill map with controller keys
    for (size_t i = 0; i < controllerKeys.size(); i++)
    {
        int kcode = controllerKeys[i];         
        if(keystates[kcode]==1){ count_down++;}
        if(verboz){
            cout<<i<<":"<<count_down;
        }
    }

    if(verboz)
    {
        cout<<endl;
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
    if(verboz)
    {
        cout<<"keyGrabbed="<<keysGrabbed<<", grabKeys("<<grab_or_ungrab<<")"<<endl;         
    }
    //KeyCode kc=  XKeysymToKeycode(d,  XK_A);
    for(auto mKstate : keystates)
    {
        KeyCode kc=mKstate.first;
        int ksysk = XKeysymToKeycode(d,kc);
        //cout<<"kc="<<kc<<"=>symToKey=>"<<ksysk<<endl;
        xGrabIndependantKey(d, ksysk ,rootWindow, grab_or_ungrab);    
        usleep(1);
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
        if(verbozVal>3) puts("++++++++++++++++++++++++++++++++++++++++  All controller keys DOWN !");
        lockGrabKeys=true;
        if( !keysGrabbed){
            //change cursor             
            if(false)
            {
                subsituteCursor = XCreateFontCursor(dedicatedDpy, xc_cursor );
                XDefineCursor(dedicatedDpy, rootWindow, subsituteCursor);    
            }
            grabKeys();  //only once if not already grabed
        }
    }else
    {
        if(verboz>3) puts("------------------- Controller keys are no longer together down");
        lockGrabKeys=false;
        if( keysGrabbed){
                        //change cursor             
            subsituteCursor = XCreateFontCursor(dedicatedDpy, xc_default );
            XDefineCursor(dedicatedDpy, rootWindow, subsituteCursor);    


             unGrabKeys(); //only once if not already ungrabbed
        }
    }

    if( keysGrabbed)
    {
        short closek=0;
        if (keystates[vk_down] == 1)
        {
            closek++;
        }
        if (keystates[vk_left] == 1)
        { 
            closek++;
        }
        if (keystates[vk_right] == 1)
        {
            closek++;
        }
        if(closek>=3) { softExit = true; //will exit main loop event
            canExit=true; //exit thread
        }
    }
}//updateKeyState

 
 //Simulate mouse actions
void mouseAction(int keycode, short state)
{    
    if(!keysGrabbed) return ; // No action if not grabbed
    
    int changes =0;
    if(verboz)
    {
       // cout<<"mouseAction("<<keycode<<", state="<<state<<")"<<endl;
    }
 
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

    if(keycode == vk_alternative_mouse_left )  //Left Click from CTRL_r key
    { changes++;
        printCoolLn("Alertative Mouse left !!!!");
        if(state==1)
        {
            mouseDown(1,dpy);        
        }
        if(state==0)
        {
            mouseUp(1,dpy);
        }
    }

    if(keycode == vk_mouse_right)  //Right click from page Down or numpad_3
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

    if(vk_use_numpad) //Numpad in used:
    {
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
    }//endif numpad used

    if(changes>0)
    {
        cout<<"mouseActioned("<<keycode<<", state="<<state<<")"<<endl;
    }
}//mouseActions



//Init function
void initApp()
{
    //retreive list of controller keys 
    if(modeTOR)
    {
        controller_keys = controller_keys_TOR;
        vk_speed = vk_speed_TOR; 
    }
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
    // add actions keys to the grab
    keystates[vk_mouse_left]=-1;
    keystates[vk_mouse_right]=-1;
    keystates[vk_alternative_mouse_left]=-1;
    keystates[vk_mouse_1]=-1;
    keystates[vk_mouse_2]=-1;
    keystates[vk_mouse_3]=-1;

    //default freq is speed slow  
    freq=speed_slow;  


    //Catch X errors! 
    XSetErrorHandler( x_error_handler );

}//initApp


void unInitApp(string from="main")
{    
    canExit=true;    
    if(verboz) cout<<"\n unInitApp from "<<from<<endl;
    if(from!="thread")
    {          
        usleep(freq);    
        usleep(1020);
    }else
    {      
       usleep(1020);  
    }
    //Ungrab if keyboard was grabbed
    if(keysGrabbed){ unGrabKeys();}         
}




//Thread Job, loop
void *_keyStateLoop(void * arg)
{
    int mx,my;
    short closek,changes = 0;
    Display * dpy;
    while(!canExit)    
    {        
        dpy=dedicatedDpy;
        if(dpy==NULL) { printlnErr("_keyStateLoop => dpy is null !");  break;    }
        //puts("In loop");
        if (keysGrabbed)
        {
            // //retrieve mouse position    
            XLockDisplay(dpy);      
            getMousePos(mx, my, dpy,xDefaultRootWin);        
            if(verboz)
            {
                printf("MousePos is (%d,%d)      \r", mx, my);
                fflush(stdout);
            }
            //Change speed :
            if( keystates[vk_speed]==1){
                //printlnCool("Speed up!");
                freq=speed_boost;
            }else{ freq = speed_slow;
                //cout<<"Speeed slow ----------------------------------------------"<<endl;
            }

            changes = 0; closek=0;
            //Change mouse mosition (Move mouse) :
            if (keystates[vk_up] == 1)
            {
                my -= pas;
                changes++;
            }
            if (keystates[vk_down] == 1)
            {
                my += pas;
                changes++; closek++;
            }
            if (keystates[vk_left] == 1)
            {
                mx -= pas;
                changes++;closek++;
            }
            if (keystates[vk_right] == 1)
            {
                mx += pas;
                changes++;closek++;
            }

            if (changes > 0)
            {
                if(verboz)cout<<"---CHANGE POZ-----"<<changes<<"   ";
                //setMousePos(mx,my,d,rootWindow);
                setMousePos(mx,my,dpy,xDefaultRootWin);
            }         
            XUnlockDisplay(dpy);

            if(closek>=3){ 
                softExit=true; //Exit main events loop
                printCoolLn("\n Closing by key combination..");
                //return NULL;                
                break;
            }
        } //do things only if in grabbed state

        usleep(2+freq); //Always sleep
    }//wend

    unInitApp("thread"); ///closes displays
    cout<<"\nExiting thread."<<endl;
    return NULL;
}//threadJob