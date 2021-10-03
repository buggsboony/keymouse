#!/bin/bash

# console colors 2021-10-03 11:04:37
GREEN='\033[0;32m'   #printf "${LRED}File not found :'${1}'${NC}\n"
LGREEN='\033[1;32m'  #printf " Set executable rights for : ${YELL}\"$realpath\" ${NC}\n"
WHITE='\033[1;37m'
YELL='\033[1;33m'
RED='\033[0;31m'
LRED='\033[1;31m'
MAG='\033[0;35m'
LMAG='\033[1;35m'
CYAN='\033[0;36m'
LCYAN='\033[1;36m'
NC='\033[0m' # No Color


#install stuff
what=${PWD##*/}   
extension=
subdir=cpp

realpath=$PWD/$subdir/$what$extension

printf " Set executable rights for : ${YELL}\"$realpath\" ${NC}\n"

chmod +x $realpath

echo Symbolic link creation
echo "/usr/bin/$what to $realpath"
sudo ln -s "$realpath" /usr/bin/$what
echo "done."
