#!/bin/bash

#install stuff
#install stuff
what=${PWD##*/}   
extension=
subdir=cpp

realpath=$PWD/$subdir/$what$extension
 
echo "killing running instances"
killall $what

echo "remove symbolic link from usr bin"
sudo rm /usr/bin/$what 

echo "done."

