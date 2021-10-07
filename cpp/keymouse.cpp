#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>   //   g++ "-lX11 -lXtst"
#include <stdio.h>
#include <ctype.h>
#include <csignal>
#include <unistd.h>


#include <pthread.h>
#include <iostream>

using namespace std;


#include "keymouse.h"



void signalHandlerClose(int sig)
{
    cout<<"Termination signal: "<<sig<<endl;
     
    cout<<"Closing App requested."<<endl;
    unInitApp("main");
    exit(sig);
}//sigHandler

int main(int argc, char **argv)
{
  string help="     Options: \n"
 "\n\t-h, --help                      Displays help on commandline options.                                    "
 "\n\t -v, --version                  Displays version information.                                           "
//"\n\t --author                        Afficher les informations sur auteur.                                    "
//"\n\t --license                       Afficher les informations sur la licence.                                    "
 "\n\t--printkey                      Display key codes on keypressed                                           "
 "\n\t--reset, --reset-config         Drop and build config file                                               "
 "\n\t--config, --open-config         Open config file   "
 "\n\t--click, --test-click           Test: sends mouse events after 2sec, down for 1 sec then release   "
 "\n\t--verbose                       Logs                                          "
;
    for (size_t i = 0; i < argc; i++)
    {
        string arg=argv[i];
        if(arg=="--printkey")
        {
            printKeyMode=true;
        }
        if( (arg=="--reset") || (arg=="--reset-config") )
        {
            resetConfig=true;
        }
        if( (arg=="--config") || (arg=="--open-config") )
        {
            openConfig=true;
        }

        if( (arg=="--click") || (arg=="--test-click") )
        {            
            testClick=true;
        }

        if( (arg=="-v") || (arg=="--version") )
        {
            cout<<appCodeName<<" "<<version<<endl;
            return 0;
        } 
        
        if( (arg=="-h") || (arg=="--help") )
        {
            cout<<help<<endl;
            return 0;
        } 
        
        if(  (arg=="--verbose") )
        {
            verboz=true;
        }
        
    }//next arg


    //Greetings, version and name
    printlnCool(appTitle+" "+version);


    //Register for signals exits
    signal(SIGTERM, signalHandlerClose);
    signal(SIGINT, signalHandlerClose);
    
////-------------  Récupération de la config  ------------------ 

//read write config file   /home/userxxx/.config/keymouse/keymouse.conf
string configFullPath=getHomeDir(); configFullPath.append("/.config/"+appCodeName+"/"); 
createDirectory(configFullPath, true); //ensure directories exist
string configFullName = configFullPath.append(configFileBaseName);

if(resetConfig) removeFile(configFullName);

ConfigFile cfile(configFullName); 
 
if( (fileExists(configFullName)) && (cfile.read() == 0)  ) 
{ 
    //cout<<"Read!"<<endl;
    //Speed parameters
    speed_boost=cfile.getiVar("speed_boost");//le port server du client	
    speed_slow=cfile.getiVar("speed_slow");//le port server du client	      
    //ControlerKeys - hotkeys => will enable grab actions
    controller_keys=cfile.getVar("controller_keys");
    //Key to enable speed boost
    vk_speed=cfile.getiVar("key_speed");
    //Keys to move mouse
    vk_up=cfile.getiVar("vk_up");
    vk_left=cfile.getiVar("vk_left");
    vk_down=cfile.getiVar("vk_down");
    vk_right=cfile.getiVar("vk_right");
    //Keys to simulates mouse Buttons for mouse events
    vk_mouse_left=cfile.getiVar("vk_mouse_left"); 
    vk_mouse_right=cfile.getiVar("vk_mouse_right");
    vk_alternative_mouse_left=cfile.getiVar("vk_alternative_mouse_left"); 
    vk_use_numpad=cfile.getbVar("vk_use_numpad"); 
    vk_mouse_1=cfile.getiVar("vk_mouse_1"); 
    vk_mouse_2=cfile.getiVar("vk_mouse_2");
    vk_mouse_3=cfile.getiVar("vk_mouse_3");  
}else
{  
    //cout<<"Write!"<<endl;
    cfile.writeVar("controller_keys",controller_keys);
    cfile.writeiVar(vk_speed,"key_speed"); 
    cfile.writeiVar(vk_up,"vk_up"); 
    cfile.writeiVar(vk_left,"vk_left"); 
    cfile.writeiVar(vk_down,"vk_down"); 
    cfile.writeiVar(vk_right,"vk_right"); 
    //speed params
    cfile.writeiVar(speed_slow,"speed_slow"); 
    cfile.writeiVar(speed_boost,"speed_boost"); 
    //Mouse Buttons for mouse events
    cfile.writeiVar(vk_mouse_left,"vk_mouse_left"); 
    cfile.writeiVar(vk_mouse_right,"vk_mouse_right");
    cfile.writeiVar(vk_alternative_mouse_left,"vk_alternative_mouse_left"); 
    cfile.writebVar(vk_use_numpad,"vk_use_numpad"); 
    cfile.writeiVar(vk_mouse_1,"vk_mouse_1"); 
    cfile.writeiVar(vk_mouse_2,"vk_mouse_2");
    cfile.writeiVar(vk_mouse_3,"vk_mouse_3");  

    printCoolLn("Nouveau fichier config de config créé : '"+configFileBaseName+"'");
}

if(openConfig)
{
    cout<<"Opening config file :"<<endl<<configFullName<<endl;
    string command="xdg-open "; command.append(configFullName); command.append("&");
    execute(command);
    cout<<"Done"<<endl;
    return 2;
}
//init application here
initApp();

    XInitThreads(); //allow concurrent threads
    d = XOpenDisplay(NULL);
    dedicatedDpy = XOpenDisplay(NULL);
    xDefaultRootWin = XDefaultRootWindow(dedicatedDpy);

    rootWindow = DefaultRootWindow(d);
    Window curFocus;
    char buf[17];
    KeySym ks;
    XComposeStatus comp;
    int len;
    int revert;


    XGetInputFocus (d, &curFocus, &revert);
    XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask |FocusChangeMask      );


if(testClick)
{
    cout<<"Test click will start"<<endl;
      sleep(2);
           mouseDown(1,dedicatedDpy);        
            sleep(1);
                mouseUp(1,dedicatedDpy);
            cout<<"Test click stopped"<<endl;            
    cout<<"Done"<<endl;
    return 2;
}

 //starting Thread process: 
 int err = pthread_create(&tid, NULL, &_keyStateLoop, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n keyStateLoop is running.\n");



    while (!canExit)
    {
        usleep(1); //never be carefull enough
        XEvent ev;
        if(d==NULL){ printlnErr("Display * d is null"); break;}
        XNextEvent(d, &ev);
        switch (ev.type)
        {
            case FocusOut:
                if(verboz) {
                     printf ("Focus changed!\n");
                     printf ("Old focus is %d\n", (int)curFocus);
                }
                if (curFocus != rootWindow)
                    XSelectInput(d, curFocus, 0);
                XGetInputFocus (d, &curFocus, &revert);
                if(verboz) printf ("New focus is %d\n", (int)curFocus);
                if (curFocus == PointerRoot)
                    curFocus = rootWindow;
                //puts("XSelectInput");
                //XGrabKey(d, XKC_COMMA, 0, curFocus, True, GrabModeAsync, GrabModeAsync);
                XSelectInput(d, curFocus, KeyPressMask|KeyReleaseMask|FocusChangeMask);
                break;
            
            case KeyRelease: 
                len = XLookupString(&ev.xkey, buf, 16, &ks, &comp);
                if(printKeyMode)
                {
                    printf ("KeyRelease :\n");                   
                    printf ("KeyCode is: %d ---- ", (int)ks);
                    if (len > 0 && isprint(buf[0]))
                    {
                        buf[len]=0;
                        printf("String is: %s\n", buf);
                    }                    
                }else
                {
                    int keycode=(int)ks;
                    updateKeyState(keycode, 0);
                    mouseAction(keycode,0);
                }
                break;

            case KeyPress:            
                len = XLookupString(&ev.xkey, buf, 16, &ks, &comp);
                if(printKeyMode)
                {
                    printf ("KeyPress :\n");                    
                    printf ("KeyCode is: %d ---- ", (int)ks);
                    if (len > 0 && isprint(buf[0]))
                    {
                        buf[len]=0;
                        printf("String is: %s\n", buf);
                    }                    
                }//printKeyMode
                else
                { //GameMode (keymouse job)                
                    int keycode=(int)ks;
                    updateKeyState(keycode, 1);
                    mouseAction(keycode,1);
                }
                //exit by key combination :
                if(softExit)
                {                   
                    cout<<"Exit main events loop..."<<endl;
                    break;      
                }
                
        }

    }//wend canExit



    unInitApp();

    if(verboz) puts("wait for thread to exit");
    pthread_join(tid,NULL);
    
    puts(" XCloseDisplay ...");
    if(d!=NULL) { XCloseDisplay(d); d=NULL;}    
    if(dedicatedDpy!=NULL){ XCloseDisplay(dedicatedDpy); dedicatedDpy=NULL;}
    printCoolLn("Good bye...");

    return 0;
}//int main