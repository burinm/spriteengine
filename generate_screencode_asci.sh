#!/bin/bash

Z=91
A=65

#Note, lol, this is brittle
OFFSET=-64
for i in `echo -n "$1" | grep -o .`;
do
    ASCII=16#`echo -n $i | xxd -p`  # 16#, hex->decimal
    if [ $(($ASCII - $A)) -ge 0 ] && [ $(($Z - $ASCII)) -ge 0 ]
    then
        SCREENCODE=$(($ASCII + $OFFSET))
    else
        SCREENCODE=$ASCII
    fi
    #echo -n "${SCREENCODE}"
    echo  -e -n `printf '\\\x%02x' $SCREENCODE`
done;
