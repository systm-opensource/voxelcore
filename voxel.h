
// This describes one single voxel in a localized voxel grid. We don't store
// vertex and color info here itself, but we store the XYZ position of the voxel
// inside the grid, and the color to be used. This means each voxel only
// requires 16 bytes of memory on x64 systems.

#ifndef _VOXEL_VOXEL_
#define _VOXEL_VOXEL_

class voxel
{
public:
	int x;
	int y;
	int z;
	int c;
	voxel();
	voxel(int xp, int yp, int zp, int ci);
};

#endif
