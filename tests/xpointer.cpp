#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/cursorfont.h>

int main() {
  Display * dpy = XOpenDisplay(NULL);
  if(dpy == NULL) {
    fprintf(stderr, "error: could not open display.");
    exit(EXIT_FAILURE);
  }
  Cursor csr = XCreateFontCursor(dpy, XC_crosshair);
  switch(csr) {
  case BadAlloc:
    fprintf(stderr, "error: BadAlloc during XCreateFontCursor");
    exit(EXIT_FAILURE);
  case BadValue:
    fprintf(stderr, "error: BadValue during XCreateFontCursor");
    exit(EXIT_FAILURE);
  }
  Window rootw = XDefaultRootWindow(dpy);
  XEvent evt;
  int err = XGrabPointer(dpy,
  			 rootw,
  			 False,
  			 ButtonPressMask,
  			 GrabModeAsync,
  			 GrabModeAsync,
  			 None,
  			 csr,
  			 CurrentTime);
  switch(err) {
  case BadCursor:
    fprintf(stderr, "error: BadCursor during XGrabPointer");
    exit(EXIT_FAILURE);
  case BadValue:
    fprintf(stderr, "error: BadCursor during XGrabPointer");
    exit(EXIT_FAILURE);
  case BadWindow:
    fprintf(stderr, "error: BadCursor during XGrabPointer");
    exit(EXIT_FAILURE);
  }
  XNextEvent(dpy, &evt);
  printf("got it!\n");
  /*
    Display some information about the event probably...
   */
}