#!/bin/bash

Z=91
A=65

#Note, lol, this is brittle
ALPHA_OFFSET=-64
NUM_OFFSET=-64
for i in `echo -n "$1" | grep -o .`;
do
    ASCII=16#`echo -n $i | xxd -p`  # 16#, hex->decimal
    if [ $(($ASCII)) -ge $A ] && [ $(($ASCII)) -le $Z ]
    then
        SCREENCODE=$(($ASCII + $ALPHA_OFFSET))
    else
        SCREENCODE=$(($ASCII))
    fi
    #echo -n "${SCREENCODE}"
    echo  -e -n `printf '\\\x%02x' $SCREENCODE`
done;
