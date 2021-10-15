#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/cursorfont.h>

using namespace std;


int main(int argc , char ** argv)
{
    Display*    dpy     = XOpenDisplay(0);
    Window      rootWindow    = DefaultRootWindow(dpy);
   
   int cur = XC_bogosity; //default;

   
   Cursor c; 
   cout<<"Count args ="<<argc<<endl;

    if(argc>=2)
    {
        string scur = argv[1];
        cout<<"Scursor="<<scur<<endl;
        cur= atoi(scur.c_str() );
    }
   c = XCreateFontCursor(dpy,cur );
   XDefineCursor(dpy, rootWindow, c);
    XCloseDisplay(dpy);
    return 0;
}
