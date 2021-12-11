#!/bin/bash

#Note, lol, this is brittle
OFFSET=-64
for i in `echo -n "$1" | grep -o .`;
do
    ASCII=16#`echo -n $i | xxd -p`  # 16#, hex->decimal
    SCREENCODE=$(($ASCII + $OFFSET))
    #echo -n "${SCREENCODE}"
    echo  -e -n `printf '\\\x%02x' $SCREENCODE`
done;
