
// So this is one of our most central pieces of the entire thing: something I'm
// calling a localized voxel grid. It is basically the envelope for one model,
// or later on parts of a greater model. It defines how big the model is, where
// it is currently located, its rotation, and it has at least one voxel in it
// so it will be considered in the rendering cycle.

#ifndef _VOXEL_GRID_
#define _VOXEL_GRID_

#include <cstdlib>
#include <vector>
#include "fileio.h"
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
	int NodesAmount;

	// A bool that states whether or not this grid has been opened and read from
	// a file
	bool ModelOpenedFromFile;

	// Through this FileIO object, we 1) load the octree file and 2) can
	// constantly reload parts we need at the level we need, at run time, in
	// real time.
	fileio FileReader;

	// Where the CENTER point of this grid is currently located in 3D space
	vertex Position;
	// A virtual vertex that represents the on-screen coordinate of the grid
	vertex VirtualPosition;
	// Rotation of this grid
	vertex Rotation;

	// The pool of all individual colors in this grid
	std::vector<color> Colors;

	// This is a dynamic pool of voxel grid nodes, which changes every time
	// this voxel grid is rendered. These are, in essence, the single voxels.
	std::vector<voxelgridnode> Nodes;

	// Here we note the single start positions for each of the octree node
	// levels when opened from a file (which should always be the case). We do
	// this so we don't have to look for the positions of every node when we
	// begin to look for them. When loading a model from file, these vectors are
	// automatically filled. With the getNodesForLevels call, we can then simply
	// acquire the needed information from the file in the fastest possible way.
	// We also note the amount of nodes per level.
	std::vector<int> NodeLevels;
	std::vector<int> NodeLevelPositions;
	std::vector<int> NodeLevelNodes;
	// Here we note down the sizes of each voxel grid note, per level. We note
	// this down here in such an array, so that we can calculate its pixel size
	// instantly, by using its distance. This frees up the space previously used
	// by the public member "Size" in a voxelgridnode.
	std::vector<float> NodeLevelSizes;

	// This cannot have a blank constructor, so you need to create it manually
	voxelgrid(int s);
	// Loads an octree file in our format directly from file
	voxelgrid(std::string filename);

	// The destructor that clears everything within this object
	~voxelgrid();

	// Get radius of voxelgrid
	float getRadius();

	// Gets the array index for the node level information in the file
	int getNodeLevelPosition(int level);

	// Gets the nodes for the specified level.
	void getNodesForLevel(int level);
};

#endif
