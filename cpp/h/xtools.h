#ifndef XTOOLS_H

#define XTOOLS_H


#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h> //target_link_libraries(projectname X11)
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/XKBlib.h> //for XkbkeycodeToKeysym
#include <X11/Xatom.h>
 
#include <X11/Intrinsic.h> //apt install libxt-dev and  yum install libxt-devel
#include <X11/extensions/XTest.h> //sudo apt-get install libxtst-dev and sudo yum install libXtst-devel
//xtestfake  target_link_libraries(readkeyboard X11 Xtst)


#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <iostream> 
using namespace std;

#include "iotools.h"
#include "xkbd.h"


class MouseState
{
  private:
    unsigned mask=-1;
    
  public:
    int x=0,y=0;
    bool btn1_isDown=false;
    bool btn2_isDown=false;
    bool btn3_isDown=false;
    bool btn4_isDown=false;
    bool btn5_isDown=false;
    
  void setMask(unsigned m)
  {            
    btn1_isDown = (m & Button1Mask);
    btn2_isDown = (m & Button2Mask);
    btn3_isDown = (m & Button3Mask);
    btn4_isDown = (m & Button4Mask);
    btn5_isDown = (m & Button5Mask);
    mask=m;
  }
  
  unsigned getMask()
  {
    return mask;    
  }

  static bool Compare(MouseState m, MouseState m2)
  {
      bool isSame = true;
        if(m2.x != m.x) return false;
        if(m2.y != m.y) return false;
        if(m2.btn1_isDown != m.btn1_isDown) return false;
        if(m2.btn2_isDown != m.btn2_isDown) return false;
        if(m2.btn3_isDown != m.btn3_isDown) return false;
        if(m2.btn4_isDown != m.btn4_isDown) return false;
        if(m2.btn5_isDown != m.btn5_isDown) return false;
        
        return isSame;
  }
  
  bool equals(MouseState m)
  {
      return MouseState::Compare(*this,m);
  }
  
  
  //surcharge d'opérateurs.
     bool operator == (MouseState m) {
        return equals(m);     
    }
       bool operator != (MouseState m) {
        return !equals(m);     
    }
  
};


// //Utilisation:
// int x,y;
//   getMousePos(x, y);    
//   printf("coords %d,%d\n",  x, y);
void getMousePos(int &x, int &y, Display *dpy=NULL)
{
	 // unsigned int snooze_time = 100000;	
	Window root;
	Window ret_root;
	Window ret_child;
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	unsigned int mask;
 
	bool dpyCreatedHere = false;
	if(dpy == NULL)
	{
	  dpyCreatedHere = true;
	  dpy = XOpenDisplay(NULL);
	}
	root = XDefaultRootWindow(dpy);
 
	if(XQueryPointer(dpy, root, &ret_root, &ret_child, &root_x, &root_y,
					 &win_x, &win_y, &mask))
	{   
		// original version
		//    printf("root loc: %4d,%4d win loc: %3d,%3d mask: 0x%08X\n",
		//           root_x, root_y, win_x, win_y, mask);
 
		// This returns in -geometry format
		// I added \n so it actually shows something so people who test it know it works.
		x= root_x; y= root_y;
	}
	else
	{
		// your script will break with this output, send it to stderr and let the script
		// return something sensible like +10+10 
		printf("oups, pointer not found ! \n");
	}
	
	if(dpyCreatedHere)
	{
	  XCloseDisplay(dpy);
	}
}//getMousePos                       

//getCursorPosition
void getMousePosition(int &x, int &y, Display *dpy=NULL)
{
    getMousePos(x,y,dpy);
}

//utilisation:  
MouseState getMouseState(Display *dpy=NULL)
{
    bool openedHere=false;
    if(dpy==NULL)
    {
        dpy = XOpenDisplay ("");
        openedHere=true;      
    }
   MouseState ms;    
   ms.x=-1; ms.y=-1;
    if (!dpy) return ms;
      
       Window w, r = DefaultRootWindow (dpy);
    int wx, wy, rx, ry;
    unsigned m;


    int x,y;


    Window root;
	Window ret_root;
	Window ret_child;
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	unsigned int mask;
 
    root = XDefaultRootWindow(dpy);
    if(XQueryPointer(dpy, root, &ret_root, &ret_child, &root_x, &root_y,
					 &win_x, &win_y, &mask))
	{   
			x= root_x; y= root_y;    
	}
	else
	{
		// your script will break with this output, send it to stderr and let the script		
		printf("oups, pointer not found ! \n");
	}
	
	XQueryPointer (dpy, root, &r, &w, &rx, &ry, &wx, &wy, &m);
    /*
    if (m & (Button1Mask | Button2Mask |
    Button3Mask | Button4Mask |
    Button5Mask))
    printf (" AT (%d,%d) buttons = (%d %d %d) (%d %d)\n",x,y,
    !! (m & Button1Mask), !! (m & Button2Mask),
    !! (m & Button3Mask), !! (m & Button4Mask),
    !! (m & Button5Mask));
    */

    
    ms.x = x;
    ms.y = y;
    
  
    ms.setMask(m);

    if(openedHere)
    {
        XCloseDisplay(dpy);
    }
    return ms;
}

//set mouse position
void setMousePos(int x, int y, Display* dpy=NULL, Window root_window=0)
{      
    bool dpyCreatedHere = false;
    if(dpy==NULL)
    {
      dpyCreatedHere=true;
      dpy = XOpenDisplay(0);
    }
    if(root_window==0)
    {
      root_window = XRootWindow(dpy, 0);
    }
    XSelectInput(dpy, root_window, KeyReleaseMask);
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
    if(dpyCreatedHere)
    {
      XFlush(dpy); // Flushes the output
    }     
}//end setMousePos

void secureSetMousePos(int x, int y)
{  
    Window root_window=0;   
    Display* dpy= XOpenDisplay(0);        
    root_window = XRootWindow(dpy, 0);
    
    XSelectInput(dpy, root_window, KeyReleaseMask);
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);    
      
    XFlush(dpy); // Flushes the output    
    XCloseDisplay(dpy);
}//end setMousePos


void mouseClick(int button=Button1, Display *display = NULL)
{
    unsigned int downTime=100000;
	bool dpyCreatedHere = false;
	if(display == NULL)
	{ 
	  dpyCreatedHere=true; display = XOpenDisplay(NULL);	  
	}
	XEvent event;	
	if(display == NULL)
	{
		fprintf(stderr, "Oups problème d'ouverture du display\n");
		exit(EXIT_FAILURE);
	}
	
	memset(&event, 0x00, sizeof(event));
	
	event.type = ButtonPress;
	event.xbutton.button = button;
	event.xbutton.same_screen = True;
	
	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	
	event.xbutton.subwindow = event.xbutton.window;
	
	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;
		
		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}
	
	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");
	
	XFlush(display);	
	usleep(downTime); //not working properly
	event.type = ButtonRelease;
	event.xbutton.state = 0x100;
	
	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");
	XFlush(display);	
	
	if(dpyCreatedHere)
	{	
	  XCloseDisplay(display);
	}
}//end mouseClick
 

 //getForegroundWindow (keyboard focused)
Window getFocusedWindow(Display *display=NULL)
{
   bool createdHere = false;
   if(display == NULL) { display = XOpenDisplay(0);
   createdHere = true;     
   }  
   //Find the window which has the current keyboard focus.
   Window winFocus;
   int    revert;
   XGetInputFocus(display, &winFocus, &revert);

   if(createdHere)
   {         
      XCloseDisplay(display);
   }
   return winFocus;
} //end getFocusedWindow

Window getForegroundWindow(Display *d=NULL)
{    
    return getFocusedWindow(d);
}


//more maskes:
//Usually Mask1 is Alt or Meta, Mask2 is Num lock, Mask3 is AltGr, Mask4 is Win, and Mask5 is Scroll 
//on some computer Mod1 is Alt and Mod4 is Meta.
#define AltMask Mod1Mask
#define AltGrMask Mod5Mask
#define NumLockMask Mod2Mask
#define CapsLockMask Mod3Mask
#define MetaMask Mod4Mask
#define WindowsMask Mod4Mask
//#define ScrollMask Mod5Mask

//first char indicator: -62
#define Char_Sterling -93


//first char indicator: -61
#define Char_ugrave -71 //-71 ù  -103 Ù
#define Char_Ugrave -103


#define Char_agrave -96 // -96 à -128 À 
#define Char_Agrave -128 

//#define Char_Ugrave -61

int getModifierKeyMask(int xkKeyCode)
{

    int mask = 0;
    if(xkKeyCode ==  XKC_CAPSLOCK) mask = LockMask; // no effect in XSendEvent
    //if(xkKeyCode ==  XKC_NUMLOCK) mask =  0; // no effect in XSendEvent
    //if(xkKeyCode ==  XKC_CAPSLOCK) { mask = ShiftMask;}
    if( (xkKeyCode ==  XKC_LCTRL) || (xkKeyCode ==  XKC_RCTRL)  ) mask = ControlMask;
    if( (xkKeyCode ==  XKC_LSHIFT) || (xkKeyCode ==  XKC_RSHIFT)  ) { mask = ShiftMask; }
    
    //  ShiftMask | LockMask | ControlMask |Mod5Mask |Mod3Mask| Mod4Mask  | Mod2Mask  | Mod1Mask;
    return mask;
}

//convert keycode to sympol then check if its a modifier key
bool XkIsModifierKey(int xkKeyCode, Display * dpy = NULL)
{ 
    bool openedHere = false;
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
      openedHere=true;
    }
    bool result = IsModifierKey(  XkbKeycodeToKeysym(dpy, xkKeyCode,0,0)  );
    if(openedHere)
    {
        XCloseDisplay(dpy);
    }
    return result;
}


// Function to create a keyboard event
XKeyEvent createKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int keycodeToBeConverted, int modifiers)
{
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycodeToBeConverted);
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}



// Function to create a keyboard event
XKeyEvent XCreateKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int bruteEventKeycode, int modifiers)
{
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = bruteEventKeycode;
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}

//Not working with firefox window, get the window caption or title
string getWindowName(Window w, Display* d=NULL){
  
  bool createdHere = false;
  if(d == NULL) { d = XOpenDisplay(0);
   createdHere = true;     
   }  
   
  XTextProperty prop;
  Status s,status;
  string name="";
  if(!w) return "";
  try{
      //char *buf;  XFetchName(d,w,&buf);
      
    s = XGetWMName(d, w, &prop); // see man
    int i; char **list;
    status = XmbTextPropertyToTextList(d, &prop, &list, &i);
    if (status >= Success && i && *list)
    {
      name = (char*) *list;
    }
  }catch(...)
  {
      printf("oups, error i, getWindowName");    
  }
   if(createdHere)
   {         
      XCloseDisplay(d);
   } 
  return name;
}


#define VK_space XK_space
#define VK_Enter XK_KP_Enter
//send virtual key to focused window:
//Utilisation XSendKey(XK_KP_Enter);
void XSendKey(int vk, Window winFocus=0, int timeDown = 0, Display *display = NULL, Window winRoot = 0)
{
   bool createdHere = false;
   if(display == NULL) { display = XOpenDisplay(0);
   createdHere = true;     
   }  
   // Get the root window for the current display.
   if(winRoot==0)
   {
     winRoot = XDefaultRootWindow(display);
   }
   
   if(winFocus==0)
   {
      int    revert;
      XGetInputFocus(display, &winFocus, &revert);
   }   
   // Send a fake key press event to the window.
   XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, vk, 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);   
   usleep(timeDown);
   // Send a fake key release event to the window.
   event = createKeyEvent(display, winFocus, winRoot, false, vk , 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);   
   if(createdHere)
   {         
      XCloseDisplay(display);
   }   
}// end sendKey


void XSendKeyEx(int vk, int modifiers_flags=0, Window winFocus=0, int timeDown = 0, Display *display = NULL, Window winRoot = 0)
{
   bool createdHere = false;
   if(display == NULL) { display = XOpenDisplay(0);
   createdHere = true;     
   }  
   // Get the root window for the current display.
   if(winRoot==0)
   {
     winRoot = XDefaultRootWindow(display);
   }
   
   if(winFocus==0)
   {
      int    revert;
      XGetInputFocus(display, &winFocus, &revert);
   }   
            
   // Send a fake key press event to the window.
   XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, vk, modifiers_flags);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);   
   usleep(timeDown);
   // Send a fake key release event to the window.
   event = createKeyEvent(display, winFocus, winRoot, false, vk , modifiers_flags);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);   
   if(createdHere)
   {         
      XCloseDisplay(display);
   }   
}// end sendKey



//send brut key code to window
void XSendKeyCodeEvent(int eventKeycode, bool down, int modifiers_flags=0,  Window winFocus=0, Display *display = NULL, Window winRoot = 0)
{
   bool createdHere = false;
   if(display == NULL) { display = XOpenDisplay(0);
   createdHere = true;     
   }  
   // Get the root window for the current display.
   if(winRoot==0)
   {
     winRoot = XDefaultRootWindow(display);
   }
   
   if(winFocus==0)
   {
      int revert;
      XGetInputFocus(display, &winFocus, &revert);
   }   
   // Send a fake key press event to the window.
   XKeyEvent event = XCreateKeyEvent(display, winFocus, winRoot, down, eventKeycode, modifiers_flags);
   XSendEvent(event.display, event.window, True, (down)?KeyPressMask:KeyReleaseMask, (XEvent *)&event);   
   
   if(createdHere)
   {         
      XCloseDisplay(display);
   }   
}// end XSendKeyCode



#define sendKey XSendKey
#define sendkey XSendKey

string enumWindows_(map<Window,string> *windowsHashTable=NULL, Display *display = NULL, Window rootWindow=0)
{   

string output ="";
   bool createdHere = false;
   if(display == NULL) { display = XOpenDisplay(0);
   createdHere = true;     
   }  
   if(rootWindow==0)
   {
      int screen = DefaultScreen (display);
      rootWindow = RootWindow (display, screen);  
   }
 
   static int level = 0;
   Window parent;
   Window *children;
 
   unsigned int noOfChildren;
   int status;
   int i;
   
   XTextProperty wmName;
   char **list;
   
   string name="";

   status = XGetWMName (display, rootWindow, &wmName);
   if ((status) && (wmName.value) && (wmName.nitems))
   {
      status = XmbTextPropertyToTextList (display, &wmName, &list, &i);
      if ((status >= Success) && (i) && (*list))
      {
	name=(char*) (*list);	
	//printf("window %d, text:[%s]\n",(int) rootWindow, name.c_str());
	output += name+"\n";
	if(windowsHashTable!=NULL)
	{
	  (*windowsHashTable)[rootWindow] = name;
	}
      }
   }
   
   level++;
   
   status = XQueryTree (display, rootWindow, &rootWindow, &parent, &children, &noOfChildren);
   
   for (i=0; i < noOfChildren; i++)
   {
      string subwin = enumWindows_(windowsHashTable, display, children[i]);     
      output += (subwin);
   }
   
   XFree ((char*) children);
   
   if(createdHere)
   {         
      XCloseDisplay(display);
   } 
   return output;
}//end enumWindows


vector <string> enumWindows(map<Window,string> *windowsHashTable=NULL, Display *display = NULL, Window rootWindow=0)
{
  string separated = enumWindows_(windowsHashTable, display, rootWindow);
  return splitStr(separated,'\n');  
}//end enum windows

// END GET THE ACTUAL TEXT!!! - This is where it all comes unstuck


// END ENUMERATE WINDOWS
Window findWindow(string name)
{
    map<Window,string> windowsHashTable;
  vector<string> windows = enumWindows( &windowsHashTable);
  
  map<Window,string>::iterator it;
		    it=windowsHashTable.begin();
	      for (; it != windowsHashTable.end(); it++ )
	      {		
		Window wnd = (*it).first;
		  string vv =(*it).second;
		   if(vv==name)
		   {
		     return wnd;
		   }
	      }
	      return 0;
}//end findWindow

/*
 
 XWindowAttributes *xwa = new XWindowAttributes();
           XGetWindowAttributes(display, window, xwa);
           
 */

//send a key state
//utilisation:
//
//int code =XK_Down; sendKeyState(code,true, display); sendKeyState(code,false, display);
// int code = XStringToKeysym("Z");
int sendKeyState(int xk_code, bool isdown, Display* dpy=NULL)
{
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }
   int res = XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, xk_code  ), isdown, CurrentTime);
   XFlush(dpy);
   return res;
}

//up or down button
int sendButtonState(int button, bool isdown, Display* dpy=NULL)
{
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }
   int res = XTestFakeButtonEvent(dpy, button , isdown, CurrentTime);
   XFlush(dpy);
   return res;
}
  
       
       //mouseDown(); sleep(4); mouseUp();
int mouseDown(int button = 1, Display* dpy=NULL)
{
   return sendButtonState(button,true,dpy); 
}//mousedown

int mouseUp(int button = 1, Display* dpy=NULL)
{
   return sendButtonState(button,false,dpy); 
}//mousedown

//mouseClick(1,2000); //down for 2 secs
void mouseClick(int button, int delay, Display* dpy=NULL)
{
     if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }   
   mouseDown(button,dpy);
   usleep(delay*1000);
   mouseUp(button,dpy);
}//mousedown

void mouseLeftClick(int delay=190, Display* dpy=NULL)
{    
     int button = 1;
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }   
   mouseDown(button,dpy);
   usleep(delay*1000);
   mouseUp(button,dpy);
}

//mouseRightClick();
void mouseRightClick(int delay=190, Display* dpy=NULL)
{    
     int button = 3;
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }   
   mouseDown(button,dpy);
   usleep(delay*1000);
   mouseUp(button,dpy);
}

void mouseDblClick(int button = 1, int delay = 150, Display* dpy=NULL)
{ 
    if(dpy==NULL)
    {             
      dpy = XOpenDisplay(0);
    }   
    mouseClick(button,60,dpy);
    usleep(delay * 1000);
    mouseClick(button,60,dpy);
}//mouseDblClick


//amélorations possibles (mémoire et quand release, envoyer info release)
//if btn1_wasDown then send event Release
void secureSetMouseState(MouseState ms)
{  
    Window root_window=0;   
    Display* dpy= XOpenDisplay(0);        
    root_window = XRootWindow(dpy, 0);
    
    XSelectInput(dpy, root_window, KeyReleaseMask);
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, ms.x, ms.y);    
    
    XTestFakeButtonEvent(dpy, 1 , ms.btn1_isDown, CurrentTime);
    XTestFakeButtonEvent(dpy, 2 , ms.btn2_isDown, CurrentTime);
    XTestFakeButtonEvent(dpy, 3 , ms.btn3_isDown, CurrentTime);
    
    XFlush(dpy); // Flushes the output   
    XCloseDisplay(dpy);
}//end setMousePos
 

 
 
 //-------- convert char code to XK Code  -----------
int charToXK(int c,int c2, int & single_modifiers_flags)
{
    int xk=-1,cx=0;
    int cbase = 0;
    int xbase = 0;
    
    
    single_modifiers_flags=0;
    
    //numbers
    if( (c>=(int)'0')&&(c<=(int)'9') ) { cbase=(int)'0'; xbase = XK_0;         
        cx = c-cbase;
        xk=xbase+cx;
        single_modifiers_flags |= ShiftMask;
    }
    //letters
    if( (c>=(int)'a')&&(c<=(int)'z') ) { cbase=(int)'a'; xbase = XK_a;        
        cx = c-cbase;
        xk=xbase+cx;        
    }
    if( (c>=(int)'A')&&(c<=(int)'Z') ) { cbase=(int)'A'; xbase = XK_A;        
        cx = c-cbase;
        xk=xbase+cx;
        single_modifiers_flags |= ShiftMask;
    }
    
    if(c==(int)'-') xk=XK_minus;
    if(c==(int)'+'){ xk=XK_plus; single_modifiers_flags |= ShiftMask;}
    if(c==(int)'='){ xk=XK_equal;}
    if(c==(int)'/'){ xk=XK_slash;single_modifiers_flags |= ShiftMask;  }
    if(c==(int)'*'){ xk=XK_asterisk;}
     
    if(c==(int)','){ xk=XK_comma;}
    if(c==(int)':'){ xk=XK_colon; }
    if(c==(int)';'){ xk=XK_semicolon;}    
    if(c==(int)'%'){ xk=XK_percent; single_modifiers_flags |= ShiftMask; }    
    
    if(c==(int)' '){ xk=XK_space;}
    if(c==(int)'\n'){ xk=XK_Return;}
    if(c==(int)'!'){ xk=XK_exclam;}
    if(c==(int)'?'){ xk=XK_question;single_modifiers_flags |= ShiftMask; }
    if(c==(int)'$'){ xk=XK_dollar; }
 
 
 if(c==-61)//first char indicator
 {
    
     if( c2==(int)Char_agrave){ xk=XK_agrave; } // 'à' 
     if( c2==(int)Char_Agrave){ xk=XK_Agrave; single_modifiers_flags |= CapsLockMask;  } // 'À' failed  
     
     if(c2==(int)Char_ugrave){ xk=XK_ugrave; } // 'ù' 
     if(c2==(int)Char_Ugrave){ xk=XK_Ugrave; single_modifiers_flags |= ShiftMask; } // 'Ù' failed
 }
 if(c==-62)//first char indicator
 {     
      if(c2==(int)Char_Sterling){ xk=XK_sterling; single_modifiers_flags |= ShiftMask; } // '£'
 }
    
    
    if(c==(int)'('){ xk=XK_parenleft; }
    if(c==(int)')'){ xk=XK_parenright; }
    if(c==(int)'.'){ xk=XK_period;single_modifiers_flags |= ShiftMask;  }    
    if(c==(int)'\''){ xk=XK_apostrophe; } 
    if(c==(int)'"'){ xk=XK_quotedbl; } 
    if(c==(int)'<'){ xk=XK_less;  } 
    if(c==(int)'>'){ xk=XK_greater; single_modifiers_flags |= ShiftMask;  } 
    
    if(c==(int)'{'){ xk=XK_braceleft; single_modifiers_flags |= AltGrMask;   } 
    if(c==(int)'}'){ xk=XK_braceright; single_modifiers_flags |= AltGrMask;   } 
    
    if(c==(int)'['){ xk=XK_bracketleft; single_modifiers_flags |= AltGrMask;   } 
    if(c==(int)']'){ xk=XK_bracketright; single_modifiers_flags |= AltGrMask;   } 
    if(c==(int)'\\'){ xk=XK_backslash; single_modifiers_flags |= AltGrMask;   } 
    
   /*

    if(c==(int)'''){ xk=XK_apostrophe; }
    
     //XK_Ccedilla
     XK_degree
  XK_asciicircum
  XK_Acircumflex
  XK_asciitilde
    XK_numbersign
   XK_ampersand                     0x0026 U+0026 AMPERSAND 
     XK_quoteright      deprecated
      //*/   
      
      //Debug here
  
  //printf("charToXK: Ci=%d, C=%c,XK=0x%x\n",c,c, xk);
    return xk;
}

 
 
 
 
 
//get pixel
XColor getPixel(int x,int y,int &r, int &g,int &b, Display *dpy=NULL, Window root_window=0)
{    
    XColor c;
    r=g=b=-1;
    if(dpy==NULL)
    {            
       dpy = XOpenDisplay((char *) NULL);
    }
    if(root_window==0)
    {
      root_window = XRootWindow(dpy, 0);
    }         
   
    XImage *image;
    image = XGetImage (dpy, XRootWindow (dpy, XDefaultScreen (dpy)), x, y, 1, 1, AllPlanes, XYPixmap);
    c.pixel = XGetPixel (image, 0, 0);
    XFree (image);
    XQueryColor (dpy, XDefaultColormap(dpy, XDefaultScreen (dpy)), &c);
    r=(c.red/256);
    g=(c.green/256);
    b=(c.blue/256);
    return c;
}//getpixel

    /*
      // Fake the pointer movement to new relative position
  XTestFakeMotionEvent (dpy, 0, event.xbutton.x + 100,
  event.xbutton.y + 50, CurrentTime);
  XSync(dpy, 0);
  sleep(3);
     */
    
    
 /*       //set color Xlib
  *  Display * disp = XOpenDisplay("");
    Colormap cmap = DefaultColormap(disp,screen);    
    XColor xcolour;
    xcolour.flags = DoRed | DoGreen | DoBlue;
    xcolour.red=65000; xcolour.green=0; xcolour.blue=0;    
    XAllocColor(disp,cmap, &xcolour);
    XSetForeground(disp,gc,xcolour.pixel);
    */

 
void XSetColor(int r,int g, int b, GC gc, Colormap cmap,Display* disp)
{
      XColor xcolour;
    xcolour.flags = DoRed | DoGreen | DoBlue;
    xcolour.red=r; xcolour.green=g; xcolour.blue=b;    
    XAllocColor(disp,cmap, &xcolour);
    XSetForeground(disp,gc,xcolour.pixel);    
}
#endif
