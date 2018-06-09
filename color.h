
// Describes a color in the RGBA space, with which we then can paint a pixel or
// rather a voxel point appropriately.

#ifndef _VOXEL_COLOR_
#define _VOXEL_COLOR_

#include <cstdlib>

class color
{
public:
	int r;
	int g;
	int b;
	int a;
	color();
	color(int rv, int gv, int bv);
	color(int rv, int gv, int bv, int av);
};

#endif

