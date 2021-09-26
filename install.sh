#!/bin/bash

#install stuff
what=${PWD##*/}   
extension=.py
subdir=py

echo "Dependances (pip)"
pip install pynput 

realpath=$PWD/$subdir/$what$extension
echo "Set executable rights for : \"$realpath\" "
chmod +x $realpath

echo Symbolic link creation
echo "/usr/bin/$what" -> "$realpath"
sudo ln -s "$realpath" /usr/bin/$what
echo "done."
