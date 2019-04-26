#!/bin/bash
gcc -Wall `pkg-config fuse --cflags` $1 -o AFSHiaAP_B10 `pkg-config fuse --libs`