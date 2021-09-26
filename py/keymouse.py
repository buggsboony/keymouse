#!/bin/env python
from threading import Thread
import time

from pynput.keyboard import Key, Listener
from pynput.mouse import Button, Controller
#pip install pynput 
#https://pynput.readthedocs.io/en/latest/mouse.html

mouse = Controller()

#put your keys here
vk2=Key.ctrl #Ctrl
vk1=Key.cmd  #Meta key (windows key)
vk_speed=Key.alt #Alt Key  for more speed

vk_up=Key.up 
vk_left=Key.left 
vk_down=Key.down 
vk_right=Key.right

vk_mouse_left=Key.ctrl_r #Ctrl_r default for click, for right click, might use thre dedicated button
vk_mouse_1='1'   #can use num pad 1, 2 , 3 to control mouse buttons
vk_mouse_2='2'
vk_mouse_3='3'

#Controller keys states
key_state_vk1=''
key_state_vk2=''
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
pas=1 #step pixel mouse
uspeed_max=200 #depends on your CPU.. suggest: uspeed=1700 for i7   ,   uspeed=900 for i3
uspeed_slow=2001
detectRepeatValue=3000


#Variable 
keyRepeatValue=dict() #array assoc
keyRepeatValue[vk_right]=False
elapsedKeyTime=0



#******************  Functions 
def usleep(microsec):
    time.sleep(microsec/1000000.0)



#********************************* Thread Job 
def job(arg):
    global uspeed_max,uspeed_slow,pas,key_state_vkSpeed, key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down    
    global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3
    while(True):
        #print('JOB: ',)
        changes=False
        if( (key_state_vk1==1) and (key_state_vk1==1)  ):                    
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
                
        if( keyRepeatValue[vk_right] ): #start to count
            elapsedKeyTime+=1
            #print("keyrepeat val RIGHT=", elapsedKeyTime)

        if(key_state_vkSpeed==1):
            uspeed=uspeed_max
        else:
            uspeed=uspeed_slow
                         

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
    print('{0} pressed'.format(key))
    updateKeyStates(key,1)
    mouseActions(key,'pressed')
    #print( (key_state_vk1) )
    
def on_release(key):
    updateKeyStates(key,0)
    mouseActions(key,'release')
    #print(vars(key))
    # if( hasattr(key, 'char') and hasattr(key, 'vk') ):
    #     if( (key.vk is None) and key.char =='2'  ):
    #         # Stop listener
    #         return False





def updateKeyStates(key, down_up):
    global vk_speed,key_state_vkSpeed, vk_right, key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
    global vk_mouse_left,vk_mouse_1, vk_mouse_2, vk_mouse_3
    global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3

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
           
    #global key_mouse_state_left, key_mouse_state_1,key_mouse_state_2,key_mouse_state_3
    if( key == vk_mouse_left):
       if(pressed_released=='pressed'):
            mouse.press(Button.left)        #print('Oura down button with Ctrl_r')    
       else:
            mouse.release(Button.left)      #print('Oura release button with Ctrl_r')
    if( keyIsChar(key, vk_mouse_1) ):
        if(pressed_released=='pressed'):
            mouse.press(Button.left)        #print('Oura down button with Ctrl_r')    
        else:
            mouse.release(Button.left)      #print('Oura release button with Ctrl_r')

    # global key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
    # print('vk1stat= ',key_state_vk1, ' vk2stat= ',key_state_vk2 )
    # if( (key_state_vk1 == 1) and ( key_state_vk2 == 1)  ):
    #    if( (key_state_up == 1) ):
    #         print('Col Up')
    # else :
    #    print ('Control off')



#Start thread
t=Thread(target=job, args=('Go',) )
t.start()



# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release,
        suppress= True #use Suppress = true to prevent event propagation
        ) as listener:
    listener.join()