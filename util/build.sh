#!/bin/bash

g++ -std=c++11 -fpermissive -O2 -w -Wfatal-errors -I../ \
../color.cpp \
../fileio.cpp \
../vertex.cpp \
../voxelgridnode.cpp \
modelconv.cpp -o ./modelconv
