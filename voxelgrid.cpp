
#include "voxelgrid.h"

voxelgrid::voxelgrid(int w, int h, int d)
{
	// Define size
	size_w=w; size_h=h; size_d=d;
}


void voxelgrid::clearVoxels()
{ Voxels.clear(); }


std::vector<vertex> voxelgrid::GetOutlineVertices()
{
	// The vertices
	std::vector<vertex> v;

	// X Radius
	float r_x = (float)size_w / 2;
	// Y Radius
	float r_y = (float)size_h / 2;
	// Z Radius
	float r_z = (float)size_d / 2;

	// OK! We can now generate the vertices
	
	vertex v1(Position.x - r_x, Position.y - r_y, Position.z + r_z);
	vertex v2(Position.x + r_x, Position.y - r_y, Position.z + r_z);
	vertex v3(Position.x + r_x, Position.y + r_y, Position.z + r_z);
	vertex v4(Position.x - r_x, Position.y + r_y, Position.z + r_z);
	vertex v5(Position.x - r_x, Position.y - r_y, Position.z - r_z);
	vertex v6(Position.x + r_x, Position.y - r_y, Position.z - r_z);
	vertex v7(Position.x + r_x, Position.y + r_y, Position.z - r_z);
	vertex v8(Position.x - r_x, Position.y + r_y, Position.z - r_z);

	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);
	v.push_back(v5);
	v.push_back(v6);
	v.push_back(v7);
	v.push_back(v8);

	return v;
}
