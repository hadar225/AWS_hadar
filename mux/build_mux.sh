#!/bin/bash

cd /mnt/c/Users/Hadar/Desktop/projForAWS/forMux/Mpeg2-TS
# Use native gcc, NOT aarch64-linux-gnu-gcc, unless you want ARM64 target
gcc -g -DUINT='unsigned int' -D__int32=int -D__int8=char -D__int16=short -w -c Mpeg2-TS.c
ar cru libmux.a Mpeg2-TS.o
cp libmux.a /mnt/c/Users/Hadar/Desktop/projForAWS/forMux/mux
