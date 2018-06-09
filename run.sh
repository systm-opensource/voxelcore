#!/bin/bash

rm -rf ./voxelcore

g++ \
	camera.cpp \
	color.cpp \
	display.cpp \
	events.cpp \
	framebuffer.cpp \
	functions.cpp \
	rectangle.cpp \
	pixel.cpp \
	vertex.cpp \
	voxel.cpp \
	voxelgrid.cpp \
	main.cpp \
-I/usr/include -I./ -std=c++11 -w -fpermissive -Wfatal-errors -o ./voxelcore -lSDL2

./voxelcore
