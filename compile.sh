#!/bin/bash

gcc -Wall `pkg-config fuse --cflags` $1 -o fuse `pkg-config fuse --libs`