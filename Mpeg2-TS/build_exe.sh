#!/bin/bash

gcc -DUINT='unsigned int' -D__int32=int -D__int8=char -D__int16=short -w -g -lm  Mpeg2-TS.c -o M -lm
