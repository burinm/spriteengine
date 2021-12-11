#!/bin/bash

SAMPLE=sample_matrix.bg
TEXT_OFFSET=10
TEXT_ROW_OFFSET=10
CHARACTERS_PER_ROW=40

rm -f $SAMPLE

for i in `seq 0 39`
    do echo -n -e \\x2a >> $SAMPLE;  # * asters
done

for j in `seq 0 27`
do
    echo -n -e \\x2a >> $SAMPLE  # * asters
    for i in `seq 0 37`
        do echo -n -e \\x20 >> $SAMPLE;  # [ ] spaces
    done
    echo -n -e \\x2a >> $SAMPLE;  # * asters
done

for i in `seq 0 39`;
    do echo -n -e \\x2a >> $SAMPLE;  # * asters
done

eval ./generate_screencode_asci.sh "SPRITEENGINE_V1" | dd of=$SAMPLE bs=1 seek=$(($TEXT_OFFSET * $CHARACTERS_PER_ROW + $TEXT_OFFSET)) conv=notrunc
