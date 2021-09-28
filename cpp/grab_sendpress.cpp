#include <X11/Xlib.h> //  /usr/include/X11/Xlib.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

void SendPressKeyEvent(Display *display, XKeyEvent xkey)
{
    Window current_focus_window;
    int current_focus_revert;
    XGetInputFocus(display, &current_focus_window, &current_focus_revert);
    xkey.type =  KeyPress;
    xkey.display = display;
    xkey.window = current_focus_window;
    xkey.root = DefaultRootWindow(display);
    xkey.subwindow = None;
    xkey.time = 1000 * time(0);
    xkey.x = 0;
    xkey.y = 0;
    xkey.x_root = 0;
    xkey.y_root = 0;
    xkey.same_screen = True;
    XSendEvent(display, InputFocus,  True, KeyPressMask, (XEvent *)(&xkey));
}

void SendReleaseKeyEvent(Display *display, XKeyEvent xkey)
{
    Window current_focus_window;
    int current_focus_revert;
    XGetInputFocus(display, &current_focus_window, &current_focus_revert);
    xkey.type =  KeyRelease;
    xkey.display = display;
    xkey.window = current_focus_window;
    xkey.root = DefaultRootWindow(display);
    xkey.subwindow = None;
    xkey.time = 1000 * time(0);
    xkey.x = 0;
    xkey.y = 0;
    xkey.x_root = 0;
    xkey.y_root = 0;
    xkey.same_screen = True;
    XSendEvent(display, InputFocus, True, KeyReleaseMask, (XEvent *)(&xkey));
}

void *TaskCode(void* arg)
{
    switch(*(int*)arg)
    {
    case 38:
        system("espeak -v en "  "\"a\"");
    }
    return 0;   
}

int main()
{
    Display *display = XOpenDisplay(0);
    if(display == 0)
        exit(1);
    XGrabKeyboard(display, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync, CurrentTime);
    XEvent event;
    while(true)
    {
        XNextEvent(display, &event);
        if(event.type == Expose)
        {

        }
        if(event.type == KeyPress)
        {
            SendPressKeyEvent(display,event.xkey);
            if(event.xkey.keycode == 38)
            {
                pthread_t thread;
                int thread_arg = event.xkey.keycode;
                pthread_create(&thread,0, TaskCode, (void*) &thread_arg);
            }
        }
        if(event.type == KeyRelease)
            SendReleaseKeyEvent(display,event.xkey);
    }
    XCloseDisplay(display);
}