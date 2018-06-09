
// Describes the most fundamental structure and object there is in just about
// any 3D simulation of any kind: the vertex.
// Has two constructors: default uses a zero position, the second lets you
// define the vertex directly.

#ifndef _VOXEL_VERTEX_
#define _VOXEL_VERTEX_

#include <cstdlib>

class vertex
{
public:
	float x;
	float y;
	float z;
	vertex();
	vertex(float xp, float yp, float zp);
};

#endif

