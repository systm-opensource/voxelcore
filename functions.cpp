
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


// Adds one single voxel to the specified grid. Function here also to reduce
// memory needed for each grid.
void addVoxelToGrid(voxel v, voxelgrid *g)
{
	// Make sure the voxel does not outside the grid bounds
	if (v.x <= g->size_w && v.y <= g->size_h && v.z <= g->size_d)
	{ g->Voxels.push_back(v); }
}


// Adds a complete bunch of voxels to the specified grid
void addVoxelsToGrid(std::vector<voxel> v, voxelgrid *g)
{
	for (int i=0; i<=v.size(); i++)
	{
		if (v[i].x <= g->size_w && v[i].y <= g->size_h && v[i].z <= g->size_d)
		{ g->Voxels.push_back(v[i]); }
	}
}


// Determines if a voxel grid is within the frustum of the camera
bool isVoxelgridInFrustum(voxelgrid *g, camera *cm)
{
	// By default it's not... we want to increase performance.
	bool inFrustum = true;

	// Temp coords
	vertex vgp(g->Position.x, g->Position.y, g->Position.z);

	// Adjust
	vgp.x -= cm->Position.x;
	vgp.y -= cm->Position.y;
	vgp.z -= cm->Position.z;

	// Only consider what lies ahead of us
	if (vgp.z < 0.0f) { inFrustum = false; }
	else
	{

	}

	// Is it visible?
	return inFrustum;
}



// Draws the outline of a voxel grid
void drawVoxelgridOutline(voxelgrid *g, framebuffer *fb, camera *cm, int fov)
{
	if (isVoxelgridInFrustum(g, cm) == true)
	{
		int cx = fb->ScreenWidth() / 2;
		int cy = fb->ScreenHeight() / 2;
		std::vector<vertex> v = g->GetOutlineVertices();
		std::vector<pixel> p;

		for (int i=0; i<v.size(); i++)
		{
			v[i].x -= cm->Position.x;
			v[i].y -= cm->Position.y;
			v[i].z -= cm->Position.z;

			float f = (float)fov / v[i].z;

			pixel np = project(cx, cy, v[i].x, v[i].y, f);
			p.push_back(np);
		}

		// OK... render it
		color c(255, 0, 0, 255);
		fb->drawLine(p[0], p[1], c);
		fb->drawLine(p[3], p[2], c);
		fb->drawLine(p[4], p[5], c);
		fb->drawLine(p[7], p[6], c);
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


// So this generates the vertex position for a voxel inside a localized voxel
// grid. For that we need the voxel itself and some info about the grid
// itself. We do not want to copy in the entire grid to avoid slowdown issues.
// The entire grid contains a lot more information which we don't need for 
// vertex generation.

