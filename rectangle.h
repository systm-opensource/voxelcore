
// Describes a rectangular area on the screen - not in 3D space.

#ifndef _VOXEL_RECTANGLE_
#define _VOXEL_RECTANGLE_

#include <cstdlib>

class rectangle
{
public:
	// The four corners of the rectangle
	int x;
	int y;
	int w;
	int h;
	rectangle();
	rectangle(int xp, int yp, int wv, int hv);
};

#endif
