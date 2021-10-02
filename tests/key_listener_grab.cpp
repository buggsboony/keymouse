#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>   //   g++ "-lX11 -lXtst"
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>


#include <iostream>
using namespace std;

#define XKC_COMMA 0x3A
int main ()
{
    Display* d = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(d);
    Window curFocus;
    char buf[17];
    KeySym ks;
    XComposeStatus comp;
    int len;
    int revert;



        unsigned int    modifiers       = ControlMask | ShiftMask ;
                        int             keycode         = XKeysymToKeycode(d,XK_A);
                        Window          grab_window     =  root;
                        Bool            owner_events    = False;
                        int             pointer_mode    = GrabModeAsync;
                        int             keyboard_mode   = GrabModeAsync;
  //modifiers= Mod2Mask;  //ATTENTION à VERNUM (Mod2Mask)



    XGetInputFocus (d, &curFocus, &revert);
    XSelectInput(d, curFocus, KeyPressMask|KeyReleaseMask |FocusChangeMask      );

    while (1)
    {
        puts("usleep");
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

            case KeyPress:
            //

                printf ("Got key!\n");
                len = XLookupString(&ev.xkey, buf, 16, &ks, &comp);
                if (len > 0 && isprint(buf[0]))
                {
                    buf[len]=0;
                    printf("String is: %s\n", buf);
                    string s="u";
                    if(s==buf)
                    {                        
                        puts("Cool now => ungrab!");
                        XUngrabKey(d,keycode,modifiers,root);
                    }
                    s="g";
                    if(s==buf)
                    {
                           
                    
                  
                         XGrabKey(d, keycode, modifiers, root, owner_events, pointer_mode, keyboard_mode);
                        puts("Woauwh  Grab !!! ");
                    }
                }
                else
                {
                    printf ("Key is: %d\n", (int)ks);
                }
        }

    }
}//int main