
// This is the off-screen, in-memory framebuffer, which contains the information
// of the pixels to be blit into the SDL texture when everything is finished.
// In my actual engine, there will be more than one framebuffer - but as this
// is the sandbox for my actual engine, we'll only use this one buffer.

#ifndef _VOXEL_FRAMEBUFFER_
#define _VOXEL_FRAMEBUFFER_

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "color.h"
#include "pixel.h"
#include "rectangle.h"

class framebuffer
{
public:
	// This is the background color square so we don't have to "refill" the
	// entire buffer with this color again and again... instead we simply do
	// a memcpy before we render the pixels :)
	unsigned char *BackgroundPixels;

	// This is the actual pixel data that gets sent into the SDL texture
	unsigned char *Pixels;

	// The background color to be used
	color BackgroundColor;

	// The FOV to be used
	int FOV;

	// Needs screen dimensions to allocate the buffer
	framebuffer(int w, int h);

	// Set the background color
	void setBackgroundColor(color bg);

	// Clear the buffer prior to rendering
	void clear();

	// Resize the buffer. Used when the window resizes, for example
	void resizeBuffer(rectangle newSize);
	
	// Sets the color of one particular pixel to the specified color
	void setPixelToColor(int x, int y, color c);

	// Draws a line between two points with the specified color
	void drawLine(pixel p1, pixel p2, color c);

	// Screen width
	int ScreenWidth();
	// Screen Height
	int ScreenHeight();

private:
	// The screen rectangle
	rectangle ScreenSize;
	// The number of pixels being used
	int numberPixels;

	// Finds the exact position of a pixel in the pixel data
	int _indexOfPixel(int x, int y);

	// Blends a pixel color with another color
	color _blendColors(color c1, color c2);

	// Finds out if a pixel has a value already
	bool _pixelUsed(pixel p);
};

#endif
