#!/bin/env python
import sys
from threading import Thread
import time

from pynput.keyboard import Key, Listener
from pynput.keyboard import Controller as KeyboardController
from pynput.mouse import Button, Controller as MouseController
#pip install pynput 
#https://pynput.readthedocs.io/en/latest/mouse.html

keyboard=KeyboardController()
mouse = MouseController()

#put your keys here
vk2=Key.ctrl #Ctrl
vk1=Key.cmd  #Meta key (windows key)
vk_speed=Key.alt #Alt Key  for more speed

vk_up=Key.up 
vk_left=Key.left 
vk_down=Key.down 
vk_right=Key.right

vk_mouse_left=Key.ctrl_r #Ctrl_r default for click, for right click, might use thre dedicated button
vk_use_numpad=True
vk_mouse_1=Key.end   #can use num pad 1, 2 , 3 to control mouse buttons
vk_mouse_2='2'
vk_mouse_3='3'

#Controller keys states
key_state_vk1=''  #Controller 1
key_state_vk2=''  #Controller 2
key_state_vkSpeed=''

#Direction keys states
key_state_up=''
key_state_left=''
key_state_down=''
key_state_right=''
key_mouse_state_left=''
key_mouse_state_1=''
key_mouse_state_2=''
key_mouse_state_3=''

#variables params or const: 
verboz=True
pas=1 #step pixel mouse
uspeed_max=200 #depends on your CPU.. suggest: uspeed=1700 for i7   ,   uspeed=900 for i3
uspeed_slow=2801
detectRepeatValue=3000


#Variables - core
keyRepeatValue=dict() #array assoc
keyRepeatValue[vk_right]=False
elapsedKeyTime=0
gl_listener= None #global listener
recording = False
exitApp = False
allowPropagation=True


#******************  Functions 
def usleep(microsec):
    time.sleep(microsec/1000000.0)



#********************************* Thread Job 
def job(arg):
    global uspeed_max,uspeed_slow,pas,key_state_vkSpeed, key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down    
    global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3
    global recording, exitApp
    while(True):
        #print('JOB: ',)
        changes=False
        if( (key_state_vk1==1) and (key_state_vk2==1) and (key_state_left==1) and (key_state_down==1) and (key_state_right==1)  ):     
            print('Stop thread ') 
            exitApp=True  #Exit app
            sys.exit()  #exit thread 
        if( (key_state_vk1==1) and (key_state_vk2==1)  ):                  
            recording=True
            allowPropagation=False                                           
            mx=mouse.position[0]
            my=mouse.position[1]            
            if(key_state_up==1):
                my-=pas
                changes=True
            if(key_state_down==1):
                my+=pas
                changes=True
            if(key_state_left==1):  #LEFT KEY action
                mx-=pas
                changes=True
            if(key_state_right==1):  #RIGHT KEY action
                mx+=pas
                changes=True             
            if(changes):
                mouse.position = (mx, my)  #update cursor position
        else :
            recording = False
            allowPropagation=True
        
        if(key_state_vkSpeed==1):
            uspeed=uspeed_max
        else:
            uspeed=uspeed_slow
                         
        #print('uspeed:',uspeed)
        usleep(uspeed) #cpu sleep 200µsec
    








def keyIsChar(key, char_val):
    if( hasattr(key, 'char') and hasattr(key, 'vk') ):        
        if( (key.vk is None) and key.char == char_val  ):
            return True
        else:
            return False
    else:
        return False


def on_press(key):
    global verboz
    if(verboz):
        print('{0} pressed'.format(key))
    updateKeyStates(key,1)
    mouseActions(key,'pressed')
    #keyboardPropagate(key,'release') #Does not work !   
    
def on_release(key):
    global verboz
    updateKeyStates(key,0)
    mouseActions(key,'release')
    #keyboardPropagate(key,'release') #Does not work. 
    if(verboz):
        print(vars(key))
    # if( hasattr(key, 'char') and hasattr(key, 'vk') ):
    #     if( (key.vk is None) and key.char =='2'  ):
    #         # Stop listener
    #         return False





def updateKeyStates(key, down_up):
    global vk_speed,key_state_vkSpeed, vk_right, key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
    global vk_mouse_left,vk_mouse_1, vk_mouse_2, vk_mouse_3
    global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3
    global exitApp
    if(exitApp):
        print('Exit app now!')
        sys.exit() #Exit main thread
    if( key == vk_speed):
       key_state_vkSpeed=down_up  #store speed key state
    if( key == vk1):
       key_state_vk1=down_up
    if( key == vk2):
       key_state_vk2=down_up
    if( key == vk_up):
       key_state_up=down_up
    if( key == vk_down):
       key_state_down=down_up
    if( key == vk_left):
       key_state_left=down_up  #LEFT  key state
    if( key == vk_right):
       key_state_right=down_up #RIGHT key state 
    if( key == vk_mouse_left):
       key_mouse_state_left=down_up #Ctrl_r for left button state
    if( key == vk_mouse_1):
       key_mouse_state_1=down_up # NUM pad 1 key state
    if( key == vk_mouse_2):
       key_mouse_state_2=down_up # NUM pad 1 key state
    if( key == vk_mouse_3):
       key_mouse_state_3=down_up # NUM pad 1 key state
#  mouse.press(Button.left)
# mouse.release(Button.left)


def mouseActions(key,pressed_released):
    global vk_mouse_left,vk_mouse_1, vk_mouse_2, vk_mouse_3 
    global recording,vk_use_numpad
    #print('is recording? ', recording)
    if(not recording):        
        return None #No action with mouse
    #global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3
    if( key == vk_mouse_left):
       if(pressed_released=='pressed'):
            mouse.press(Button.left)        #print('Oura down button with Ctrl_r')                          
       else:
            mouse.release(Button.left)      #print('Oura release button with Ctrl_r')
    
    if(vk_use_numpad):
        if( (key==vk_mouse_1) or keyIsChar(key, vk_mouse_1) ):
            if(pressed_released=='pressed'):
                mouse.press(Button.left)        #print('Oura down button with Ctrl_r')    
            else:
                mouse.release(Button.left)      #print('Oura release button with Ctrl_r')



def keyboardPropagate(key,pressed_released):   
    global keyboard, allowPropagation
    if( not allowPropagation):
        return "nothing to send"
        
    if(pressed_released=='pressed'):
        keyboard.press(key)
    if(pressed_released=='release'):
        keyboard.release(key)



#Start thread
t=Thread(target=job, args=('Go',) )
t.start()



# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release,
        suppress= False #use Suppress = true to prevent event propagation (Trying to change suppress dynamically)
        ) as listener:
    listener.join()