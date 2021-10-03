#!/bin/bash

#install stuff
#install stuff
what=${PWD##*/}   
extension=.py
subdir=py

realpath=$PWD/$subdir/$what$extension
 
echo "killing running instances"
killall $what

echo "remove symbolic link from usr bin"
sudo rm /usr/bin/$what

echo "You can manually run:   pip uninstall pynput "

echo "done."

