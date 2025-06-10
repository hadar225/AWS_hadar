#!/bin/bash

aarch64-linux-gnu-gcc -DUINT='unsigned int' -D__int32=int -D__int8=char -D__int16=short -w -c  Mpeg2-TS.c
