
// These are functions that we constantly need, but do not want to place in
// their respective classes in order to save memory allocated for each class...
// It does not make sense to have 2400 colors stored for a voxel model, and then
// have 2400 times exactly the same function in memory. This process is called
// deduplication or much simpler put: optimization. And since we are testing
// a voxel engine here, we want as much memory as there can be for our actual
// data to be rendered.

#include <math.h>
#include <iostream>
#include "defines.h"
#include "camera.h"
#include "color.h"
#include "pixel.h"
#include "vertex.h"
#include "voxelgrid.h"
#include "framebuffer.h"
#include "font.h"


// A simple function to convert the given degree to radians
float toRadians(float deg)
{
	float r = (deg * PI) / 180.0f;
	return r;
}


// Provides the distance between two points in 3D space. This is mostly used
// for depth and frustum testing.
float distanceBetween(vertex v1, vertex v2)
{
	float dist = sqrt(pow(v1.x-v2.x,2.0f) + pow(v1.y-v2.y, 2.0f) + pow(v1.z-v2.z, 2.0f));
	return dist;
}


// Rotates a vertex
float* rotate(float x, float y, float rad)
{
	float* pos = new float[2];
	float s = sin(rad);
	float c = cos(rad);
	pos[0] = x*c-y*s;
	pos[1] = y*c+x*s;
	return pos;
}

// Projects a vertex into the correct pixel location
// cx = Center X of screen
// cy = Center Y of screen
// vx = Vertex X
// vy = Vertex Y
// f  = Field of View / Vertex Z
pixel project(int cx, int cy, float vx, float vy, float f)
{
	float x = vx * f;
	float y = vy * f;
	pixel p((int)cx+x, (int)cy+y);
	return p;
}


// Prepares the screen position in world space of a grid
void adjustVoxelgridPosition(voxelgrid *g, framebuffer *fb, camera *cm)
{
	// Temp coords
	vertex vgp(g->Position.x, g->Position.y, g->Position.z);

	// First rotate the grid...
	float *t1 = rotate(vgp.x, vgp.z, g->Rotation.y);
	vgp.x = t1[0]; vgp.z = t1[1];
	float *t2 = rotate(vgp.y, vgp.z, g->Rotation.x);
	vgp.y = t2[0]; vgp.z = t2[1];

	// THEN translate correctly
	vgp.x -= cm->Position.x;
	vgp.y -= cm->Position.y;
	vgp.z -= cm->Position.z;
	
	// AND rotate again! This time it's camera rotation
	float *t3 = rotate(vgp.x, vgp.z, cm->Rotation.y);
	vgp.x = t3[0]; vgp.z = t3[1];
	float *t4 = rotate(vgp.y, vgp.z, cm->Rotation.x);
	vgp.y = t4[0]; vgp.z = t4[1];

	// Delete the allocated t's
	delete t1; delete t2; delete t3; delete t4;

	// Set the values of the temp vertex
	g->VirtualPosition.x = vgp.x;
	g->VirtualPosition.y = vgp.y;
	g->VirtualPosition.z = vgp.z;

	// VGP = Virtual Grid Position
}


// Adjusts any array of vertices to screen space
vertex* adjustVerticesToScreenPosition(vertex *v, int vertamount, voxelgrid *g, framebuffer *fb, camera *cm)
{
	// The set...
	vertex *adj = new vertex[vertamount]; 

	// Adjust verts...
	for (int i=0; i<vertamount; i++)
	{
		// Temp vertex
		//vertex t = v[i];
		vertex *t = new vertex();
		t->x = v[i].x;
		t->y = v[i].y;
		t->z = v[i].z;

		// First rotate the grid...
		float *t1 = rotate(t->x, t->z, g->Rotation.y);
		t->x = t1[0]; t->z = t1[1];
		float *t2 = rotate(t->y, t->z, g->Rotation.x);
		t->y = t2[0]; t->z = t2[1];

		// THEN translate...
		t->x -= cm->Position.x;
		t->y -= cm->Position.y;
		t->z -= cm->Position.z;

		// AND rotate again! This time it's camera rotation
		float *t3 = rotate(t->x, t->z, cm->Rotation.y);
		t->x = t3[0]; t->z = t3[1];
		float *t4 = rotate(t->y, t->z, cm->Rotation.x);
		t->y = t4[0]; t->z = t4[1];
		
		// Delete the calculation addresses
		delete t1; delete t2; delete t3; delete t4;

		// Set the vertex position
		adj[i] = vertex(t->x, t->y, t->z);
		delete t;
	}

	// The set of vertices
	return adj;
}


// Gets the outline vertices of a voxelgrid, located on zero point
vertex* getOutlineVerticesOfGrid(voxelgrid *g)
{
	// The vertices
	//std::vector<vertex> v;
	vertex *v = new vertex[8];

	// Radius
	float r = ((float)g->GridSize / 2) * VOXELSIZE;

	// OK! We can now generate the vertices
	v[0].x = 0.0f - r; v[0].y = 0.0f - r; v[0].z = 0.0f + r;
	v[1].x = 0.0f + r; v[1].y = 0.0f - r, v[1].z = 0.0f + r;
	v[2].x = 0.0f + r; v[2].y = 0.0f + r; v[2].z = 0.0f + r;
	v[3].x = 0.0f - r; v[3].y = 0.0f + r; v[3].z = 0.0f + r;
	v[4].x = 0.0f - r; v[4].y = 0.0f - r; v[4].z = 0.0f - r;
	v[5].x = 0.0f + r; v[5].y = 0.0f - r, v[5].z = 0.0f - r;
	v[6].x = 0.0f + r; v[6].y = 0.0f + r; v[6].z = 0.0f - r;
	v[7].x = 0.0f - r; v[7].y = 0.0f + r; v[7].z = 0.0f - r;

	return v;
}


// Determines if a voxel grid is within the frustum of the camera
bool isVoxelgridInFrustum(voxelgrid *g, camera *cm, framebuffer *fb)
{
	bool inFrustum = true;

	// Only consider what's actually in our frustum
	float f = (float)fb->FOV / g->VirtualPosition.z;
	pixel np = project(fb->CX, fb->CY, g->VirtualPosition.x, g->VirtualPosition.y, f);
	
	float dist = distanceBetween(g->Position, cm->Position);
	
	if (np.x > 0 && np.x < fb->ScreenWidth() && np.y > 0 && np.y < fb->ScreenHeight() && g->VirtualPosition.z > g->getRadius() && dist < CAMERACLIP)
	{ inFrustum = true; }
	else
	{ inFrustum = false; }

	// Is it visible?
	return inFrustum;
}



// Draws the outline of a voxel grid
void drawVoxelgridOutline(voxelgrid *g, framebuffer *fb, camera *cm, int fov)
{
	if (isVoxelgridInFrustum(g, cm, fb) == true)
	{
		vertex *v = getOutlineVerticesOfGrid(g);
		//std::vector<vertex> v  = getOutlineVerticesOfGrid(g);
		vertex *va = adjustVerticesToScreenPosition(v, 8, g, fb, cm);
		delete v;

		pixel *p = new pixel[8];

		for (int i=0; i<8; i++)
		{	
			float f = (float)fov / va[i].z;
			p[i] = project(fb->CX, fb->CY, va[i].x, va[i].y, f);
		}

		delete va;

		// OK... render it
		color c(255, 0, 0, 255);
		fb->drawLine(p[0], p[1], c);
		fb->drawLine(p[3], p[2], c);
		fb->drawLine(p[4], p[5], c);
		fb->drawLine(p[7], p[6], c);
		fb->drawLine(p[3], p[0], c);
		fb->drawLine(p[2], p[1], c);
		fb->drawLine(p[7], p[4], c);
		fb->drawLine(p[6], p[5], c);
		fb->drawLine(p[0], p[4], c);
		fb->drawLine(p[1], p[5], c);
		fb->drawLine(p[3], p[7], c);
		fb->drawLine(p[2], p[6], c);

		delete p;
	}
}


// Writes out a string with the referenced font, into the referenced
// framebuffer, with the specified color, at the specified pixel location.
void writeString(std::string t, font *f, framebuffer *fb, color c, pixel p)
{
	// Pixel start location
	int tx = p.x;
	int ty = p.y;

	// Go through each character
	for (int i=0; i<t.length(); i++)
	{
		// ASCII code of character
		int ascii = t[i];

		// Get character position in font data
		int charpos = -1;
		for (int j=0; j<f->FontData.size(); j++)
		{
			if (f->FontData[j].ASCIICode == ascii)
			{ charpos = j; break; }
		}

		// We now know where the character is, so render it
		if (charpos != -1)
		{
			for (int j=0; j<f->FontData[charpos].X.size(); j++)
			{
				int act_x = tx + f->FontData[charpos].X[j];
				int act_y = ty + f->FontData[charpos].Y[j];
				float a_f = f->FontData[charpos].PixelAlpha[j] * 255;
				int a_i = (int)a_f;
				fb->setPixelToColor(act_x, act_y, color(c.r, c.g, c.b, a_i));
			}
			tx = tx+f->FontData[charpos].CharacterWidth;
		}
	}
}


// Traverses a voxel grid, creates the octree nodes (SVOs), and attempts to
// render out the single voxels.
void traverseVoxelGrid(voxelgrid *v, camera *c, framebuffer *f)
{
	/*
	// The current "root" node
	int curnode = 0;

	// Initial position is the center position of the grid
	vertex p = v->Position;

	// Initial dimension is the total size of the grid
	float size_w = (float)v->size_w * VOXELSIZE;
	float size_h = (float)v->size_h * VOXELSIZE;
	float size_d = (float)v->size_d * VOXELSIZE;
	vertex s(size_w, size_h, size_d);

	// And since we have eight main root nodes in the beginning, we can use this
	// as a base to perform the octree division. We will always need the first
	// eight, so that the tree generates correctly.
	while (curnode < 8)
	{

		// Increase curnode. We're done now with one root segment.
		curnode++;
	}
	*/
}


// Finds out of the voxel at the specified location is used.
bool voxelUsedInGrid(voxelgrid *g, int x, int y, int z)
{
	bool used = false;
	for (int i=0; i<g->Voxels.size(); i++)
	{
		if (g->Voxels[i].x == x && g->Voxels[i].y == y && g->Voxels[i].z == z)
		{ used = true; break; }
	}
	return used;
}


// Generates an octree of a root node and returns that list.
// List contains nodes which contains voxels only
std::vector<voxelgridnode> generateOctreeNodes(voxelgridnode *n, voxelgrid *g, camera *cm)
{
	// The vector that contains nodes with voxels
	std::vector<voxelgridnode> nodes;
	
	// A node is always half the size of its parent node, therefore:
	float size = n->Size / 2.0f;

	if (size > VOXELSIZE)
	{
		float rds = size / 2.0f;
		vertex p = n->Position;

		// No matter what, generate the eight center vertices for the subnodes
		std::vector<vertex> v;
		v.push_back(vertex( p.x-rds, p.y-rds, p.z-rds ) );
		v.push_back(vertex( p.x+rds, p.y-rds, p.z-rds ) );
		v.push_back(vertex( p.x+rds, p.y-rds, p.z+rds ) );
		v.push_back(vertex( p.x-rds, p.y-rds, p.z+rds ) );
		v.push_back(vertex( p.x-rds, p.y+rds, p.z-rds ) );
		v.push_back(vertex( p.x+rds, p.y+rds, p.z-rds ) );
		v.push_back(vertex( p.x+rds, p.y+rds, p.z+rds ) );
		v.push_back(vertex( p.x-rds, p.y+rds, p.z+rds ) );

		// Walk through the nodes and check for the existence of voxels. If there
		// are any, consider the node as a candidate. If it has voxels and is of
		// size 0.1, we are at the smallest possible size.
		for (int i=0; i<8; i++)
		{
			// Generate the node
			voxelgridnode nn(v[i]);
			nn.Parent = n;

			// Dimension ranges for this node
			float low_x, high_x, low_y, high_y, low_z, high_z;

			// Ranges
			low_x = v[i].x - rds; high_x = v[i].x + rds;
			low_y = v[i].y - rds; high_y = v[i].y + rds;
			low_z = v[i].z - rds; high_z = v[i].z + rds;

			// Check if there are voxels
			for (int j=0; j<g->Voxels.size(); j++)
			{
				// Convert voxel position to actual position
				vertex vp;
				vp.x = g->Voxels[j].x * VOXELSIZE;
				vp.y = g->Voxels[j].y * VOXELSIZE;
				vp.z = g->Voxels[j].z * VOXELSIZE;

				if (vp.x >= low_x && vp.x <= high_x && vp.y >= low_y && vp.y <= high_y && vp.z >= low_z && vp.z <= high_z)
				{  }
			}
		}
	}

	if (size == VOXELSIZE)
	{
		// We now have to check if this smallest possible node has a voxel

	}

	return nodes;
}



// Small test
void renderVoxels(voxelgrid *g, framebuffer *fb, camera *cm)
{
	/*
	std::vector<vertex> v1;
	std::vector<vertex> v2;

	for (int i=0; i<=g->Voxels.size(); i++)
	{
		vertex t((g->Voxels[i].x * VOXELSIZE) - (float)g->GridSize/2.0f * VOXELSIZE,
				 (g->Voxels[i].y * VOXELSIZE) - (float)g->GridSize/2.0f * VOXELSIZE,
				 (g->Voxels[i].z * VOXELSIZE) - (float)g->GridSize/2.0f * VOXELSIZE);
		v1.push_back(t);
	}

	v2 = adjustVerticesToScreenPosition(v1, g, fb, cm);
	v1.clear();

	for (int i=0; i<v2.size(); i++)
	{
		float f = (float)fb->FOV / v2[i].z;
		pixel np = project(fb->CX, fb->CY, v2[i].x, v2[i].y, f);
		int col = g->Voxels[i].c;
		fb->setPixelToColor(np.x, np.y, g->Colors[col]);	
	}
	v2.clear();
	*/
}
