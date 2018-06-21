
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
	// Position of this node
	vertex Position;

	// Size of this node in all dimensions
	float Size;

	// Distance to camera - this then also decides the LOD for this node
	float Distance;

	// Ranges for this node
	std::vector<float> RangeX;
	std::vector<float> RangeY;
	std::vector<float> RangeZ;

	// Parent of this node
	voxelgridnode *Parent;

	// Set the location when constructing the node
	voxelgridnode(vertex p);
};

#endif
