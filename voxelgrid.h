
// So this is one of our most central pieces of the entire thing: something I'm
// calling a localized voxel grid. It is basically the envelope for one model,
// or later on parts of a greater model. It defines how big the model is, where
// it is currently located, its rotation, and it has at least one voxel in it
// so it will be considered in the rendering cycle. 

#ifndef _VOXEL_GRID_
#define _VOXEL_GRID_

#include <cstdlib>
#include <vector>
#include "defines.h"
#include "vertex.h"
#include "voxel.h"
#include "color.h"

class voxelgrid
{
public:
	// Size of this voxel grid - can vary in each dimension.
	int size_w;
	int size_h;
	int size_d;

	// Where the CENTER point of this grid is currently located in 3D space
	vertex Position;
	// Rotation of this grid
	vertex Rotation;

	// And the voxels themselves
	std::vector<voxel> Voxels;

	// The pool of all individual colors in this grid
	std::vector<color> Colors;

	// This cannot have a blank constructor, so you need to create it manually
	voxelgrid(int w, int h, int d);
	
	// Clears the voxels when LOD changes
	void clearVoxels();

	// Get radius of voxelgrid
	float getRadius();

	// Provides the eight vertices that make up the border of the grid
	std::vector<vertex> GetOutlineVertices();
};

#endif
