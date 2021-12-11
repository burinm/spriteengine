#!/bin/bash

SAMPLE=sample_matrix.bg
TEXT_OFFSET=10
TEXT_ROW_OFFSET=10
CHARACTERS_PER_ROW=40

SCREEN_MATRIX_X=36
SCREEN_MATRIX_Y=28

rm -f $SAMPLE


for i in `seq 0 $(($SCREEN_MATRIX_X - $((1))))`
    do
        echo -n -e \\x2a >> $SAMPLE;  # * asters
done

for j in `seq 0 $(($SCREEN_MATRIX_Y - $((3))))`
do
    echo -n -e \\x2a >> $SAMPLE  # * asters
    for i in `seq 0 $(($SCREEN_MATRIX_X - $((3))))`
        do echo -n -e \\x20 >> $SAMPLE;  # [ ] spaces
    done
    echo -n -e \\x2a >> $SAMPLE;  # * asters
done

for i in `seq 0 $(($SCREEN_MATRIX_X - $((1))))`;
    do echo -n -e \\x2a >> $SAMPLE;  # * asters
done

eval ./generate_screencode_asci.sh "SPRITEENGINE_V1.0" | dd of=$SAMPLE bs=1 seek=$(($TEXT_OFFSET * $CHARACTERS_PER_ROW + $TEXT_OFFSET)) conv=notrunc
