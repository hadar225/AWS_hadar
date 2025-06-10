#!/bin/bash
  
cd /mnt/c/Users/alon\ satchi/Desktop/projForAWS/forMux/Mpeg2-TS
aarch64-linux-gnu-gcc -g -DUINT='unsigned int' -D__int32=int -D__int8=char -D__int16=short -w -c  Mpeg2-TS.c
aarch64-linux-gnu-ar cru libmux.a Mpeg2-TS.o
cp libmux.a /mnt/c/Users/alon\ satchi/Desktop/projForAWS/forMux/mux
