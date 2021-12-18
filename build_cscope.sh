#!/bin/bash
find -name "*.c" -or -name "*.h" > cscope.files

ctags -L cscope.files


