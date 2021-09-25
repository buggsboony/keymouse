#!/bin/env python
from threading import Thread
import time

from pynput.keyboard import Key, Listener
from pynput.mouse import Button, Controller
#pip install pynput 
#https://pynput.readthedocs.io/en/latest/mouse.html

mouse = Controller()

#put your keys here
vk1=Key.cmd  #Meta key (windows key)
vk2=Key.ctrl #Alt Key

vk_up=Key.up 
vk_left=Key.left 
vk_down=Key.down 
vk_right=Key.right

vk_click=Key.ctrl_r #Ctrl_r default for click
vk_click_1='1'
vk_click_2='2'
vk_click_3='3'

#Controller keys
key_state_vk1=''
key_state_vk2=''
#Direction keys
key_state_up=''
key_state_left=''
key_state_down=''
key_state_right=''


#variables: 
pas=1 #step pixel mouse
uspeed=1700 #depends on your CPU.. suggest: uspeed=1700 for i7   ,   uspeed=900 for i3
uspeed_max=200 
#******************  Functions 
def usleep(microsec):
    time.sleep(microsec/1000000.0)


#********************************* Thread Job
 

def job(arg):
    global uspeed,pas, key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
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
            if(key_state_left==1):
                mx-=pas
                changes=True
            if(key_state_right==1):
                mx+=pas
                changes=True            
            if(changes):
                mouse.position = (mx, my)  #update cursor position
                
        usleep(uspeed_max) #cpu sleep 200µsec











def on_press(key):
    print('{0} pressed'.format(key))
    updateKeyStates(key,1)
    analyse()
    #print( (key_state_vk1) )
    
def on_release(key):
    updateKeyStates(key,0)
    print(vars(key))
    if( hasattr(key, 'char') and hasattr(key, 'vk') ):
        if( (key.vk is None) and key.char =='2'  ):
            # Stop listener
            return False



def updateKeyStates(key, down_up):
    global key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
    if( key == vk1):
       key_state_vk1=down_up
    if( key == vk2):
       key_state_vk2=down_up
    if( key == vk_up):
       key_state_up=down_up
    if( key == vk_down):
       key_state_down=down_up
    if( key == vk_left):
       key_state_left=down_up
    if( key == vk_right):
       key_state_right=down_up


def analyse():
    global key_state_vk1, key_state_vk2, key_state_up, key_state_right, key_state_left, key_state_down
    print('vk1stat= ',key_state_vk1, ' vk2stat= ',key_state_vk2 )
    if( (key_state_vk1 == 1) and ( key_state_vk2 == 1)  ):
       if( (key_state_up == 1) ):
            print('Col Up')
    else :
       print ('Control off')



#Start thread
t=Thread(target=job, args=('Go',) )
t.start()



# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()