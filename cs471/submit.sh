#!/bin/bash

# Build hw string with correct 0 padding
hw="hw"
if [ ${#1} == 0 ]; then
    printf "\nPass homework name as argument.\n\n"
    exit 1
elif [ ${#1} == 1 ]; then
    hw+="0"
fi
hw+=$1


printf "\nHello $USER. Preparing $hw for submission.\n"

renamed="thung1_$hw"
tarball="tarballs/${renamed}.tar.gz"

# Rename dir, tar, then restore name
mv $hw $renamed
tar -cvzf $tarball $renamed
mv $renamed $hw

printf "\nPrinting tarball contents:\n"
tar -tzf $tarball
echo
