

// Describes a single pixel on screen. Mostly used for final projection of a
// point on screen - but also keeps the depth value of that pixel which is then
// used to solve the sorting order of rendering.

#ifndef _VOXEL_PIXEL_
#define _VOXEL_PIXEL_

#include <cstdlib>

class pixel
{
public:
	int x;
	int y;
	int d;
	pixel();
	pixel(int xp, int yp);
	pixel(int xp, int yp, int dv);
};

#endif

