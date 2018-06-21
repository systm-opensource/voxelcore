
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
#include "voxelgridnode.h"

class voxelgrid
{
public:
	// Size of this voxel grid. If this is a number of 50, this entire grid
	// has a maximum of 50x50x50 (125,000) voxels. It makes voxel models easier
	// to handle.
	int GridSize;
	long VoxelAmount;

	// Where the CENTER point of this grid is currently located in 3D space
	vertex Position;
	// A virtual vertex that represents the on-screen coordinate of the grid
	vertex VirtualPosition;
	// Rotation of this grid
	vertex Rotation;

	// And the voxels themselves
	std::vector<voxel> Voxels;

	// The pool of all individual colors in this grid
	std::vector<color> Colors;

	// This is a dynamic pool of voxel grid nodes, which changes every time
	// this voxel grid is rendered
	std::vector<voxelgridnode> Nodes;

	// This cannot have a blank constructor, so you need to create it manually
	voxelgrid(int s);
	
	// Clears the voxels when LOD changes
	void clearVoxels();

	// Get radius of voxelgrid
	float getRadius();
};

#endif
