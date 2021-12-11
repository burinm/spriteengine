#!/bin/bash
gcc -g -Wall main.c blitz_n_chips.c graphics.c machine.c screen_matrix.c spriteengine.c -I/opt/SDL/include -lSDL2
