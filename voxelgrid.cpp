
#include "voxelgrid.h"

voxelgrid::voxelgrid(int s)
{
	// Define size
	GridSize = s;
	VoxelAmount = s*s*s;
	VPUpdateNeeded = true;
}


void voxelgrid::clearVoxels()
{ Voxels.clear(); }


float voxelgrid::getRadius()
{
	// The radius
	float r = ((float)GridSize / 2) * VOXELSIZE;
	return r;
}
