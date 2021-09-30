#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>   //   g++ "-lX11 -lXtst"
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include <pthread.h>
#include <iostream>

using namespace std;


#include "keymouse_h.cpp"



int main(int argc, char **argv)
{

    int x=4;
    cout<<"hello world"<<endl;
      cout<<"close world"<<x<<endl;
      return 13;
}


int main2 (int argc, char **argv)
{

    for (size_t i = 0; i < argc; i++)
    {
        string arg=argv[i];
        if(arg=="--printkey")
        {
            printKeyMode=true;
        }
    }
    
////-------------  Récupération de la config  ------------------ 

//lire et écrire un fichier de conf
 
// string configFullPath=getHomeDir(); configFullPath.append("/.config/"+appCodeName+"/"); 
// //createDirectory(configFullPath, true); //ensure directories exist
// string configFullName = configFullPath.append(configFileBaseName);


ConfigFile cfile(configFileBaseName); //Config file is next to executable
if( cfile.read() == 0) 
{
    //cout<<"Read!"<<endl;
    speed_boost=cfile.getiVar("speed_boost");//le port server du client	
    speed_slow=cfile.getiVar("speed_slow");//le port server du client	      
    controller_keys=cfile.getVar("controller_keys");
}else
{  
    //cout<<"Write!"<<endl;
    cfile.writeVar("controller_keys",controller_keys);
    cfile.writeiVar(speed_slow,"speed_slow"); 
    cfile.writeiVar(speed_boost,"speed_boost"); 
    printCoolLn("Nouveau fichier config de config créé : '"+configFileBaseName+"'");
}

//init application here
initApp();
  
//puts("abort"); return 32;

 //starting Pipe: 
 int err = pthread_create(&tid, NULL, &_keyStateLoop, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n keyStateLoop is running.\n");












    Display* d = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(d);
    Window curFocus;
    char buf[17];
    KeySym ks;
    XComposeStatus comp;
    int len;
    int revert;


    XGetInputFocus (d, &curFocus, &revert);
    XSelectInput(d, curFocus, KeyPressMask | KeyReleaseMask |FocusChangeMask      );

    while (1)
    {
        usleep(1); //never be carefull enough
        XEvent ev;
        XNextEvent(d, &ev);
        switch (ev.type)
        {
            case FocusOut:
                printf ("Focus changed!\n");
                printf ("Old focus is %d\n", (int)curFocus);
                if (curFocus != root)
                    XSelectInput(d, curFocus, 0);
                XGetInputFocus (d, &curFocus, &revert);
                printf ("New focus is %d\n", (int)curFocus);
                if (curFocus == PointerRoot)
                    curFocus = root;
                puts("XSelectInput");
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
                }
                
        }

    }
}//int main