#ifndef XKBD_H

#define XKBD_H

#include "iotools.h"


//voir keysysmdef.h

//define some usefull Keycodes:

//{ Region xkeys manually filled

#define XKC_ESC 0x09
#define XKC_CTRL 0x25
#define XKC_LCTRL 0x25
#define XKC_WINDOWS 0x85
#define XKC_SPACE 0x41
#define XKC_CONTEXT_MENU 0x87

#define XKC_ALT 0x40
#define XKC_RCTRL 0x69
#define XKC_ALTGR 0x6C
#define XKC_LMAJ 0x32
#define XKC_RMAJ 0x3E
#define XKC_SHIFT 0x32
#define XKC_LSHIFT 0x32

#define XKC_GREATER 0x5e
#define XKC_W 0x34
#define XKC_X 0x35
#define XKC_C 0x36
#define XKC_V 0x37
#define XKC_B 0x38
#define XKC_N 0x39

#define XKC_COMMA 0x3A
#define XKC_SEMICOLON 0x3B
#define XKC_SLASH 0x3C
#define XKC_EXCLAMATION 0x3D
#define XKC_RSHIFT 0x3E


#define XKC_CAPSLOCK 0x42
#define XKC_Q 0x26
#define XKC_S 0x27
#define XKC_D 0x28
#define XKC_F 0x29
#define XKC_G 0x2A
#define XKC_H 0x2B
#define XKC_J 0x2c
#define XKC_K 0x2d
#define XKC_L 0x2e
#define XKC_M 0x2F
#define XKC_PERCENT 0x30
#define XKC_ASTERISK 0x33

#define XKC_F1 0x43
#define XKC_F2 0x44
#define XKC_F3 0x45
#define XKC_F4 0x46
#define XKC_F5 0x47
#define XKC_F6 0x48
#define XKC_F7 0x49
#define XKC_F8 0x4A
#define XKC_F9 0x4B
#define XKC_F10 0x4C
#define XKC_F11 0x5F
#define XKC_F12 0x60
#define XKC_PRINT_SCR 0x61 //supposé, non traité
#define XKC_IMPR_ECRAN 0x61 //supposition, non traité

#define XKC_ARRET_DEFIL 0x4E
#define XKC_PAUSE_ATT 0x7F

#define XKC_SQUARE 0x31
#define XKC_1 0xA
#define XKC_2 0xB
#define XKC_3 0xC
#define XKC_4 0xD
#define XKC_5 0xE
#define XKC_6 0xF
#define XKC_7 0x10
#define XKC_8 0x11
#define XKC_9 0x12
#define XKC_0 0x13
#define XKC_CELSIUS 0x14
#define XKC_PLUS 0x15
#define XKC_BACKSPACE 0x16

#define XKC_INS  0x76
#define XKC_BEGIN  0x6E
#define XKC_PAGE_UP  0x70
#define XKC_VERRNUM 0x4D
#define XKC_NUMLOCK 0x4D
#define XKC_NUM_DEVIDE  0x6A
#define XKC_NUM_MULTIPLY  0x3F
#define XKC_NUM_MINUS  0x52

#define XKC_TAB 0x17
#define XKC_A 0x18
#define XKC_Z 0x19
#define XKC_E 0x1A
#define XKC_R 0x1B
#define XKC_T 0x1C
#define XKC_Y 0x1D
#define XKC_U 0x1E
#define XKC_I 0x1F
#define XKC_O 0x20
#define XKC_P 0x21
#define XKC_CIRCUMFLEX 0x22
#define XKC_POUNDS 0x23
#define XKC_RET 0x24

#define XKC_SUPPR 0x77
#define XKC_END 0x73
#define XKC_PAGE_DOWN 0x75

#define XKC_NUM_7 0x4F
#define XKC_NUM_8 0x50
#define XKC_NUM_9 0x51
#define XKC_NUM_PLUS 0x56

#define XKC_NUM_4 0x53
#define XKC_NUM_5 0x54
#define XKC_NUM_6 0x55 

#define XKC_DEL 0x77

#define XKC_NUM_1 0x57
#define XKC_NUM_2 0x58
#define XKC_NUM_3 0x59
#define XKC_NUM_0 0x5A
#define XKC_NUM_SUPPR 0x5B

#define XKC_NUM_RET 0x68
#define XKC_F1 0x43

#define XKC_LEFT 0x71
#define XKC_UP 0x6F
#define XKC_RIGHT 0x72
#define XKC_DOWN 0x74

//}  


bool XkbState[255]; //memset(kbState,0,255)
bool CapsLock_ON =false, NumLock_ON =false;
///  This will refesh variables CapsLock_ON and NumLock_ON states
void  XGetKeyboardLockState(Display *openedDisplay = NULL)
{
    bool openedHere = false;
    if(openedDisplay ==NULL)
    {
        openedDisplay = XOpenDisplay(":0"); 
        openedHere = true;
    }
    XKeyboardState x;   
    XGetKeyboardControl(openedDisplay, &x);
        
    NumLock_ON = (x.led_mask & 2);
    CapsLock_ON = (x.led_mask & 1);
    //printf("capsloc on = %d\n NumLock_ON = %d\n",CapsLock_ON,NumLock_ON);
    if(openedHere) XCloseDisplay(openedDisplay);
}//XGetKeybardLockState
    
bool XkIsPadNumberKey(int kcode)
{
    return (kcode==0x5A //0
    || kcode==0x57
        || kcode==0x58
        || kcode==0x59
        || kcode==0x53//4
        || kcode==0x54//5
        || kcode==0x55//6
        || kcode==0x4F//7
        || kcode==0x50//8
        || kcode==0x51//9                     
            ); 
}//XkIsPadNumberKey
   
   
        
bool XkIsNumpadKey(int kcode)
{
       // 0x57 //num_1
      //0x5B .  ou Suppr
    return ( XkIsPadNumberKey(kcode) || kcode ==0x5B ); 
}//XkIsPadNumberKey
  
int numpadKeyToShiftKey(int numPadKeyCode)
{
    int shiftKcode= numPadKeyCode;
   if (numPadKeyCode==0x5A) //0
   {
        shiftKcode = 0x13;
   }
   if (numPadKeyCode==0x57) //1
   {
        shiftKcode = 0xA;
   }
   if (numPadKeyCode==0x58) //2
   {
        shiftKcode = 0xB;
   }
   
   if (numPadKeyCode==0x59) //3
   {
        shiftKcode = 0xC;
   }
   
   if (numPadKeyCode==0x53) //4
   {
        shiftKcode = 0xD;
   }
   if (numPadKeyCode==0x54) //5
   {
        shiftKcode = 0xE;
   }
   if (numPadKeyCode==0x55) //6
   {
        shiftKcode = 0xF;
   }
   if (numPadKeyCode==0x4F) //7
   {
        shiftKcode = 0x10;
   }
   if (numPadKeyCode==0x50) //8
   {
        shiftKcode = 0x11;
   }
   if (numPadKeyCode==0x51)//9
   {
        shiftKcode = 0x12;
   }
   
   if (numPadKeyCode==0x5B)//.
   {
        shiftKcode = 0x3B;
   }

   return shiftKcode;
}//numpadKeyToShiftKey
  
  
  
void setXkeyState(int xkeycode,bool b)
{
  XkbState[xkeycode] = b; //down                
}

bool isXkeyDown(int xkeycode)
{
   if(XkbState[xkeycode]) return true;
    else return false;
}


class XKbdKey
{   
    private:
        bool isDown=false;
    public:
        bool isStateSet=false;
    short index;
    char value;   
    string keyname;
    //char keyname[12];
    int xkeycode;   

    
    
    void init()
    {
       // memset(&keyname,0,12);
        index=-1;
    }
    
    XKbdKey()
    {
        init();
    }
    
    XKbdKey(int indx,int val,int xkcode=-1,string name="")
    {
        init();
        index=indx;
        value=val;
        keyname=name;
        xkeycode=xkcode;
    }
    
    string toString()
    {
        string str=keyname;
        str.append(" xkeycode="); str+=intToStr(xkeycode) ;
        str.append(" index=");
        str.append(intToStr(index));
        str.append(" value=");
        str.append(intToStr(value));
        return str;        
    }
    
    void setState(bool isdown)
    { 
        isDown = isdown; 
        isStateSet=true;
    }
    
    bool getState()
    { 
        return isDown;        
    }
};

//XKeyboardMapkeycount
//Note Il est préférable d'utiliser une seule instance de cette classe, pour éviter les atteintes des piles statiques telles que kIndexValueMap
map<  pair<short, short> ,XKbdKey> XKbd_IndexValueMap;

#define XKbd_KEYCOUNT 104

/// Utilisation de la class XKbd:
/* 
 * Display * disp = XOpenDisplay("");
 * XKbd kbd;    
    while(true)
    {
      //do Something
        msleep(10);
        kbd.queryStateMap(disp);
        if( kbd.isKeyDown(XK_Escape) )
        {
            cout<<"break on ESC"<<endl;
            break;
        }
        
    }//wend
    
    */
class XKbd
{    
    public:       
       XKbdKey keys[XKbd_KEYCOUNT];
       map<int,XKbdKey> kmap;       
       char *keys_return = new char[32];
       char *oldState = new char[32];       
    void init()
    {
        //blank fill array at initialization
        memset(keys_return,0, sizeof(char)*32);

        keys[0]=XKbdKey(1,2,XKC_ESC,"XKC_ESC"); kmap[XKC_ESC]=keys[0];
keys[1]=XKbdKey(1,4,XKC_1,"XKC_1"); kmap[XKC_1]=keys[1];
keys[2]=XKbdKey(1,8,XKC_2,"XKC_2"); kmap[XKC_2]=keys[2];
keys[3]=XKbdKey(1,16,XKC_3,"XKC_3"); kmap[XKC_3]=keys[3];
keys[4]=XKbdKey(1,32,XKC_4,"XKC_4"); kmap[XKC_4]=keys[4];
keys[5]=XKbdKey(1,64,XKC_5,"XKC_5"); kmap[XKC_5]=keys[5];
keys[6]=XKbdKey(1,128,XKC_6,"XKC_6"); kmap[XKC_6]=keys[6];
keys[7]=XKbdKey(2,1,XKC_7,"XKC_7"); kmap[XKC_7]=keys[7];
keys[8]=XKbdKey(2,2,XKC_8,"XKC_8"); kmap[XKC_8]=keys[8];
keys[9]=XKbdKey(2,4,XKC_9,"XKC_9"); kmap[XKC_9]=keys[9];
keys[10]=XKbdKey(2,8,XKC_0,"XKC_0"); kmap[XKC_0]=keys[10];
keys[11]=XKbdKey(2,16,XKC_CELSIUS,"XKC_CELSIUS"); kmap[XKC_CELSIUS]=keys[11];
keys[12]=XKbdKey(2,32,XKC_PLUS,"XKC_PLUS"); kmap[XKC_PLUS]=keys[12];
keys[13]=XKbdKey(2,64,XKC_BACKSPACE,"XKC_BACKSPACE"); kmap[XKC_BACKSPACE]=keys[13];
keys[14]=XKbdKey(2,128,XKC_TAB,"XKC_TAB"); kmap[XKC_TAB]=keys[14];
keys[15]=XKbdKey(3,1,XKC_A,"XKC_A"); kmap[XKC_A]=keys[15];
keys[16]=XKbdKey(3,2,XKC_Z,"XKC_Z"); kmap[XKC_Z]=keys[16];
keys[17]=XKbdKey(3,4,XKC_E,"XKC_E"); kmap[XKC_E]=keys[17];
keys[18]=XKbdKey(3,8,XKC_R,"XKC_R"); kmap[XKC_R]=keys[18];
keys[19]=XKbdKey(3,16,XKC_T,"XKC_T"); kmap[XKC_T]=keys[19];
keys[20]=XKbdKey(3,32,XKC_Y,"XKC_Y"); kmap[XKC_Y]=keys[20];
keys[21]=XKbdKey(3,64,XKC_U,"XKC_U"); kmap[XKC_U]=keys[21];
keys[22]=XKbdKey(3,128,XKC_I,"XKC_I"); kmap[XKC_I]=keys[22];
keys[23]=XKbdKey(4,1,XKC_O,"XKC_O"); kmap[XKC_O]=keys[23];
keys[24]=XKbdKey(4,2,XKC_P,"XKC_P"); kmap[XKC_P]=keys[24];
keys[25]=XKbdKey(4,4,XKC_CIRCUMFLEX,"XKC_CIRCUMFLEX"); kmap[XKC_CIRCUMFLEX]=keys[25];
keys[26]=XKbdKey(4,8,XKC_POUNDS,"XKC_POUNDS"); kmap[XKC_POUNDS]=keys[26];
keys[27]=XKbdKey(4,16,XKC_RET,"XKC_RET"); kmap[XKC_RET]=keys[27];
keys[28]=XKbdKey(4,32,XKC_LCTRL,"XKC_LCTRL"); kmap[XKC_LCTRL]=keys[28];
keys[29]=XKbdKey(4,64,XKC_Q,"XKC_Q"); kmap[XKC_Q]=keys[29];
keys[30]=XKbdKey(4,128,XKC_S,"XKC_S"); kmap[XKC_S]=keys[30];
keys[31]=XKbdKey(5,1,XKC_D,"XKC_D"); kmap[XKC_D]=keys[31];
keys[32]=XKbdKey(5,2,XKC_F,"XKC_F"); kmap[XKC_F]=keys[32];
keys[33]=XKbdKey(5,4,XKC_G,"XKC_G"); kmap[XKC_G]=keys[33];
keys[34]=XKbdKey(5,8,XKC_H,"XKC_H"); kmap[XKC_H]=keys[34];
keys[35]=XKbdKey(5,16,XKC_J,"XKC_J"); kmap[XKC_J]=keys[35];
keys[36]=XKbdKey(5,32,XKC_K,"XKC_K"); kmap[XKC_K]=keys[36];
keys[37]=XKbdKey(5,64,XKC_L,"XKC_L"); kmap[XKC_L]=keys[37];
keys[38]=XKbdKey(5,128,XKC_M,"XKC_M"); kmap[XKC_M]=keys[38];
keys[39]=XKbdKey(6,1,XKC_PERCENT,"XKC_PERCENT"); kmap[XKC_PERCENT]=keys[39];
keys[40]=XKbdKey(6,2,XKC_SQUARE,"XKC_SQUARE"); kmap[XKC_SQUARE]=keys[40];
keys[41]=XKbdKey(6,4,XKC_LSHIFT,"XKC_LSHIFT"); kmap[XKC_LSHIFT]=keys[41];
keys[42]=XKbdKey(6,8,XKC_ASTERISK,"XKC_ASTERISK"); kmap[XKC_ASTERISK]=keys[42];
keys[43]=XKbdKey(6,16,XKC_W,"XKC_W"); kmap[XKC_W]=keys[43];
keys[44]=XKbdKey(6,32,XKC_X,"XKC_X"); kmap[XKC_X]=keys[44];
keys[45]=XKbdKey(6,64,XKC_C,"XKC_C"); kmap[XKC_C]=keys[45];
keys[46]=XKbdKey(6,128,XKC_V,"XKC_V"); kmap[XKC_V]=keys[46];
keys[47]=XKbdKey(7,1,XKC_B,"XKC_B"); kmap[XKC_B]=keys[47];
keys[48]=XKbdKey(7,2,XKC_N,"XKC_N"); kmap[XKC_N]=keys[48];
keys[49]=XKbdKey(7,4,XKC_COMMA,"XKC_COMMA"); kmap[XKC_COMMA]=keys[49];
keys[50]=XKbdKey(7,8,XKC_SEMICOLON,"XKC_SEMICOLON"); kmap[XKC_SEMICOLON]=keys[50];
keys[51]=XKbdKey(7,16,XKC_SLASH,"XKC_SLASH"); kmap[XKC_SLASH]=keys[51];
keys[52]=XKbdKey(7,32,XKC_EXCLAMATION,"XKC_EXCLAMATION"); kmap[XKC_EXCLAMATION]=keys[52];
keys[53]=XKbdKey(7,64,XKC_RSHIFT,"XKC_RSHIFT"); kmap[XKC_RSHIFT]=keys[53];
keys[54]=XKbdKey(7,128,XKC_NUM_MULTIPLY,"XKC_NUM_MULTIPLY"); kmap[XKC_NUM_MULTIPLY]=keys[54];
keys[55]=XKbdKey(8,1,XKC_ALT,"XKC_ALT"); kmap[XKC_ALT]=keys[55];
keys[56]=XKbdKey(8,2,XKC_SPACE,"XKC_SPACE"); kmap[XKC_SPACE]=keys[56];
keys[57]=XKbdKey(8,8,XKC_F1,"XKC_F1"); kmap[XKC_F1]=keys[57];
keys[58]=XKbdKey(8,16,XKC_F2,"XKC_F2"); kmap[XKC_F2]=keys[58];
keys[59]=XKbdKey(8,32,XKC_F3,"XKC_F3"); kmap[XKC_F3]=keys[59];
keys[60]=XKbdKey(8,64,XKC_F4,"XKC_F4"); kmap[XKC_F4]=keys[60];
keys[61]=XKbdKey(8,128,XKC_F5,"XKC_F5"); kmap[XKC_F5]=keys[61];
keys[62]=XKbdKey(8,4,XKC_CAPSLOCK,"XKC_CAPSLOCK"); kmap[XKC_CAPSLOCK]=keys[62];
keys[63]=XKbdKey(9,1,XKC_F6,"XKC_F6"); kmap[XKC_F6]=keys[63];
keys[64]=XKbdKey(9,2,XKC_F7,"XKC_F7"); kmap[XKC_F7]=keys[64];
keys[65]=XKbdKey(9,4,XKC_F8,"XKC_F8"); kmap[XKC_F8]=keys[65];
keys[66]=XKbdKey(9,8,XKC_F9,"XKC_F9"); kmap[XKC_F9]=keys[66];
keys[67]=XKbdKey(9,16,XKC_F10,"XKC_F10"); kmap[XKC_F10]=keys[67];
keys[68]=XKbdKey(9,32,XKC_NUMLOCK,"XKC_NUMLOCK"); kmap[XKC_NUMLOCK]=keys[68];
keys[69]=XKbdKey(9,64,XKC_ARRET_DEFIL,"XKC_ARRET_DEFIL"); kmap[XKC_ARRET_DEFIL]=keys[69];
keys[70]=XKbdKey(9,128,XKC_NUM_7,"XKC_NUM_7"); kmap[XKC_NUM_7]=keys[70];
keys[71]=XKbdKey(10,4,XKC_NUM_MINUS,"XKC_NUM_MINUS"); kmap[XKC_NUM_MINUS]=keys[71];
keys[72]=XKbdKey(10,1,XKC_NUM_8,"XKC_NUM_8"); kmap[XKC_NUM_8]=keys[72];
keys[73]=XKbdKey(10,2,XKC_NUM_9,"XKC_NUM_9"); kmap[XKC_NUM_9]=keys[73];
keys[74]=XKbdKey(10,8,XKC_NUM_4,"XKC_NUM_4"); kmap[XKC_NUM_4]=keys[74];
keys[75]=XKbdKey(10,16,XKC_NUM_5,"XKC_NUM_5"); kmap[XKC_NUM_5]=keys[75];
keys[76]=XKbdKey(10,32,XKC_NUM_6,"XKC_NUM_6"); kmap[XKC_NUM_6]=keys[76];
keys[77]=XKbdKey(10,64,XKC_NUM_PLUS,"XKC_NUM_PLUS"); kmap[XKC_NUM_PLUS]=keys[77];
keys[78]=XKbdKey(10,128,XKC_NUM_1,"XKC_NUM_1"); kmap[XKC_NUM_1]=keys[78];
keys[79]=XKbdKey(11,1,XKC_NUM_2,"XKC_NUM_2"); kmap[XKC_NUM_2]=keys[79];
keys[80]=XKbdKey(11,2,XKC_NUM_3,"XKC_NUM_3"); kmap[XKC_NUM_3]=keys[80];
keys[81]=XKbdKey(11,4,XKC_NUM_0,"XKC_NUM_0"); kmap[XKC_NUM_0]=keys[81];
keys[82]=XKbdKey(11,8,XKC_NUM_SUPPR,"XKC_NUM_SUPPR"); kmap[XKC_NUM_SUPPR]=keys[82];
keys[83]=XKbdKey(11,64,XKC_GREATER,"XKC_GREATER"); kmap[XKC_GREATER]=keys[83];
keys[84]=XKbdKey(11,128,XKC_F11,"XKC_F11"); kmap[XKC_F11]=keys[84];
keys[85]=XKbdKey(12,1,XKC_F12,"XKC_F12"); kmap[XKC_F12]=keys[85];
keys[86]=XKbdKey(13,1,XKC_NUM_RET,"XKC_NUM_RET"); kmap[XKC_NUM_RET]=keys[86];
keys[87]=XKbdKey(13,2,XKC_RCTRL,"XKC_RCTRL"); kmap[XKC_RCTRL]=keys[87];
keys[88]=XKbdKey(13,4,XKC_NUM_DEVIDE,"XKC_NUM_DEVIDE"); kmap[XKC_NUM_DEVIDE]=keys[88];
keys[89]=XKbdKey(13,8,XKC_PRINT_SCR,"XKC_PRINT_SCR"); kmap[XKC_PRINT_SCR]=keys[89];
keys[90]=XKbdKey(13,16,XKC_ALTGR,"XKC_ALTGR"); kmap[XKC_ALTGR]=keys[90];
keys[91]=XKbdKey(13,64,XKC_BEGIN,"XKC_BEGIN"); kmap[XKC_BEGIN]=keys[91];
keys[92]=XKbdKey(13,128,XKC_UP,"XKC_UP"); kmap[XKC_UP]=keys[92];
keys[93]=XKbdKey(14,1,XKC_PAGE_UP,"XKC_PAGE_UP"); kmap[XKC_PAGE_UP]=keys[93];
keys[94]=XKbdKey(14,2,XKC_LEFT,"XKC_LEFT"); kmap[XKC_LEFT]=keys[94];
keys[95]=XKbdKey(14,4,XKC_RIGHT,"XKC_RIGHT"); kmap[XKC_RIGHT]=keys[95];
keys[96]=XKbdKey(14,8,XKC_END,"XKC_END"); kmap[XKC_END]=keys[96];
keys[97]=XKbdKey(14,16,XKC_DOWN,"XKC_DOWN"); kmap[XKC_DOWN]=keys[97];
keys[98]=XKbdKey(14,32,XKC_PAGE_DOWN,"XKC_PAGE_DOWN"); kmap[XKC_PAGE_DOWN]=keys[98];
keys[99]=XKbdKey(14,64,XKC_INS,"XKC_INS"); kmap[XKC_INS]=keys[99];
keys[100]=XKbdKey(14,128,XKC_DEL,"XKC_DEL"); kmap[XKC_DEL]=keys[100];
keys[101]=XKbdKey(15,128,XKC_PAUSE_ATT,"XKC_PAUSE_ATT"); kmap[XKC_PAUSE_ATT]=keys[101];
keys[102]=XKbdKey(16,32,XKC_WINDOWS,"XKC_WINDOWS"); kmap[XKC_WINDOWS]=keys[102];
keys[103]=XKbdKey(16,128,XKC_CONTEXT_MENU,"XKC_CONTEXT_MENU"); kmap[XKC_CONTEXT_MENU]=keys[103];

    
        //fill reverse dictionnary only one
        if(XKbd_IndexValueMap.size()<=0)
        {
            XKbdKey kk;
            for(int i=0; i<XKbd_KEYCOUNT; i++)
            {
                kk = keys[i];
                pair<short,short>p(kk.index,kk.value);
            XKbd_IndexValueMap[p]=kk;    
            }
        }
    

    }//init
    
        XKbd()
        {
            init();
        }
        
        int getKeyCount(){return XKbd_KEYCOUNT;}
        bool isKeyDown(int xkcode)
        {                    
                XKbdKey k=kmap[xkcode];              
                //cout<<"Traitement de: "<<k.toString()<<endl;
                //cout<<"keyreturns: "<<(int)keys_return[k.index]<<"]]]]"<<endl;
                if(   (keys_return[k.index] & k.value ) == k.value )
                {                    
                        keys[k.index].setState(true);
                }else 
                {
                    keys[k.index].setState(false);
                }
                return keys[k.index].getState();
        }
    
    bool isKeyDown(int index, int value)
    {
        return false;
    }
    
    //Return the changed key count if the compare parameter is true, otherwise, returns -1
    bool queryState(bool compareAndBreak=true, Display * dpy=NULL)
    {
        bool here = false;
        if(dpy==NULL)
        {
            here =true;
            dpy = XOpenDisplay(NULL);
        }
        
        if(compareAndBreak)
        {
            for(int i=0;i<32;i++)
            {
              oldState[i]=keys_return[i];
            }
        }
              
        XQueryKeymap( dpy, keys_return );      
 
        if(here)
        {
            XCloseDisplay(dpy);
        }   
        
        vector<XKbdKey> keyChanged;
        if(compareAndBreak)
        {            
            keyChanged = compareStates(oldState,keys_return,true);
            return true;//return keyChanged.size();
        }else return false;                
    }//query
    
      //Return the changed key count if the compare parameter is true, otherwise, returns -1
    vector<XKbdKey>queryStateMap(Display * dpy=NULL)
    {
       
        bool here = false;
        if(dpy==NULL)
        {
            here =true;
            dpy = XOpenDisplay(NULL);
        }
        
     
        for(int i=0;i<32;i++)
        {
          oldState[i]=keys_return[i];       
        }
     
        XQueryKeymap( dpy, keys_return );      
  
        if(here)
        {
            XCloseDisplay(dpy);
        }   
        
        vector<XKbdKey> keyChanged;
        keyChanged = compareStates(oldState,keys_return,false);
        return keyChanged;        
    }//query
        
        
        vector<XKbdKey> compareStates(char oldstate[32], char newstate[32], bool breakAtFirst)
    {    
        vector<XKbdKey> keyChanged;
        pair<short,short> p;
        for(int i = 0; i<32; i++)
        {                 
            if( oldstate[i] != newstate[i])
            {                
                int whatIsNew = oldstate[i] ^ newstate[i];
                p.first = i;                
                p.second = whatIsNew;
                //printf("map search pure key Pair<%d,%d>\n",i,whatIsNew);
                XKbdKey kchanged = XKbd_IndexValueMap[p];
                //kchanged.refreshState(this);
                kchanged.setState( isKeyDown(kchanged.xkeycode) );
                //printf("XKbd_IndexValueMap returns: [%s]\n",kchanged.keyname.c_str() );                         
                keyChanged.push_back( kchanged ) ;                
                if(breakAtFirst) break;
            }
        }//next
        return keyChanged;
    }//compareStates
    
    
    static vector<XKbdKey> CompareStates(char oldstate[32], char newstate[32], bool breakAtFirst, XKbd *globalKeyboard = NULL)
    {    
        vector<XKbdKey> keyChanged;
        pair<short,short> p;
        for(int i = 0; i<32; i++)
        {                 
            if( oldstate[i] != newstate[i])
            {                
                int whatIsNew = oldstate[i] ^ newstate[i];
                p.first = i;                
                p.second = whatIsNew;
                //printf("map search pure key Pair<%d,%d>\n",i,whatIsNew);
                XKbdKey kchanged = XKbd_IndexValueMap[p];
                //kchanged.refreshState(this);
                if(globalKeyboard!= NULL)
                {
                    globalKeyboard->isKeyDown(kchanged.xkeycode);
                }
                //kchanged.setState( isKeyDown(kchanged) );
               //printf("XKbd_IndexValueMap returns: [%s]\n",kchanged.keyname.c_str() );                         
                keyChanged.push_back( kchanged ) ;                
                if(breakAtFirst) break;
            }
        }//next
        return keyChanged;
    }//compareStates

    static bool stateChanged(char oldstate[32], char newstate[32])
    {        
        vector<XKbdKey> keyChanged = CompareStates(oldstate,newstate,true);
        int countChanged = keyChanged.size();
        return (countChanged>0);
    }//stateChanged

    

};



#endif
