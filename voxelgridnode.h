
// This is essentially a node which contains voxels. Better known as a node of an octree
// node of an SVO. If this node sticks in the parent octree vector list, if it
// does contain voxels.

#ifndef _VOXEL_VOXELGRIDNODE_
#define _VOXEL_VOXELGRIDNODE_

#include "vertex.h"
#include <vector>

class voxelgridnode
{
public:
	// Size of this node in all directions
	float Size;

	// Position of this node
	vertex Position;

	// Array position of the dominant color in this node
	int Color;

	// Set the location and dominant/average color when constructing the node
	voxelgridnode(vertex p, int c);
};

#endif
