#!/bin/bash

SAMPLE=sample_matrix.bg

rm -f $SAMPLE

for i in `seq 0 39`
    do echo -n -e \\x6a >> $SAMPLE;  # * asters
done

for j in `seq 0 27`
do
    echo -n -e \\x6a >> $SAMPLE  # * asters
    for i in `seq 0 37`
        do echo -n -e \\x60 >> $SAMPLE;  # [ ] spaces
    done
    echo -n -e \\x6a >> $SAMPLE;  # * asters
done

for i in `seq 0 39`;
    do echo -n -e \\x6a >> $SAMPLE;  # * asters
done
