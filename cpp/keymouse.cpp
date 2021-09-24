
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <unistd.h>
#include <iostream>
using namespace std;

#include <vector>

 //Not working with pure desktop window, but works with a normal window
  int main() {
    	Display *  dpy = XOpenDisplay(NULL);
            
            Window root = XDefaultRootWindow(dpy);
        //Window w = DefaultRootWindow (dpy);

puts("xsync test");
// puts("Get foreground window 5s");
// sleep(5);
//         Window w = getForegroundWindow();
        XSelectInput(dpy, root ,KeyPressMask|KeyReleaseMask);
        while(1) {
            usleep(1);
            //puts("enter while2");
XSync(dpy, 0); // to avoid chase
            while(XPending(dpy)) { //Repeats until all events are computed
            puts("xnext event ?");
            XEvent KeyEvent;
            XNextEvent( dpy,&KeyEvent); //Gets exactly one event
            
            if(KeyEvent.type==KeyPress) {
                uint32_t KeyEventCode=KeyEvent.xkey.keycode; //Gets the key code, NOT HIS CHAR EQUIVALENT
                std::cout << KeyEventCode << '\n'; //Displays the key code

                /* Code handling a Keypress event */

            } else if(KeyEvent.type==KeyRelease) {
                uint32_t KeyEventCode=KeyEvent.xkey.keycode;
                std::cout << KeyEventCode << '\n'; //Displays the key code

                /* Code handling a KeyRelease event */

            }
            }

            /* General code */

        }
}