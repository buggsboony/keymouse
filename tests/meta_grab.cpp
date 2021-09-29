#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

using namespace std;



int main()
{
    Display*    dpy     = XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;

    unsigned int    modifiers       = ControlMask | ShiftMask;
    int             keycode         = XKeysymToKeycode(dpy,XK_Y);
    Window          grab_window     =  root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

modifiers=0;
    XGrabKey(dpy, keycode, modifiers, grab_window, owner_events, pointer_mode,
             keyboard_mode);


    unsigned int    modifiers2       = ControlMask | ShiftMask;
    int             keycode2      = XKeysymToKeycode(dpy,XK_X);
 
 
    modifiers2=0;
    XGrabKey(dpy, keycode2, modifiers2, grab_window, owner_events, pointer_mode,
             keyboard_mode);

          

    XSelectInput(dpy, root, KeyPressMask );
    while(true)
    {
        bool shouldQuit = false;
        XNextEvent(dpy, &ev);
        switch(ev.type)
        {
            case KeyPress:
                cout << "Hot key pressed!" << endl;

                // XUngrabKey(dpy,keycode,modifiers,grab_window);
                // XUngrabKey(dpy,keycode2,modifiers2,grab_window);
               // shouldQuit = true;

            default:
                break;
        }

        if(shouldQuit)
            break;
    }

    XCloseDisplay(dpy);
    return 0;
}
