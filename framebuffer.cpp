
#include "framebuffer.h"

framebuffer::framebuffer(int w, int h)
{ FOV=std::min(w, h); resizeBuffer(rectangle(0,0,w,h)); }


void framebuffer::setBackgroundColor(color bg)
{
	// Note down the color
	BackgroundColor = bg;

	// Fill the "background" buffer with that color
	for (int i=0; i<numberPixels*4; i+=4)
	{
		BackgroundPixels[i+3] = 255; 		// A
		BackgroundPixels[i+2] = bg.r; 	// R
		BackgroundPixels[i+1] = bg.g;		// G
		BackgroundPixels[i+0] = bg.b;		// B
	}
}


void framebuffer::clear()
{
	memset(Pixels, 0, numberPixels*4);
	// Copy background color first
	//memcpy(Pixels, BackgroundPixels, numberPixels*4);
}


void framebuffer::resizeBuffer(rectangle newSize)
{
	// Define the screen rectangle
	ScreenSize = rectangle(0,0,newSize.w,newSize.h);

	// Determine the number of pixels to be used in the buffer
	numberPixels = (ScreenSize.w * ScreenSize.h);

	// Allocate memory for the actual buffer and background color
	BackgroundPixels = new unsigned char[numberPixels*4];
	Pixels = new unsigned char[numberPixels*4];
	memset(Pixels, 0, numberPixels*4);

	// Find FOV
	FOV = std::min(ScreenSize.w, ScreenSize.h);
}


void framebuffer::setPixelToColor(int x, int y, color c)
{
	// Skip if it is "out of bounds"
	if (x >= 0 && 
		x < ScreenSize.w &&
		y >= 0 &&
		y < ScreenSize.h)
	{
		int pix = _indexOfPixel(x, y);
		Pixels[pix+3] = 255; // No blending in first test
		Pixels[pix+2] = c.r;
		Pixels[pix+1] = c.g;
		Pixels[pix+0] = c.b;
	}
}


void framebuffer::drawLine(pixel p1, pixel p2, color c)
{
	float xdiff = (p2.x - p1.x);
	float ydiff = (p2.y - p1.y);

	if(xdiff == 0.0f && ydiff == 0.0f)
	{
		setPixelToColor(p1.x, p1.y, c);
		return;
	}

	if(fabs(xdiff) > fabs(ydiff))
	{
		float xmin, xmax;

		// set xmin to the lower x value given
		// and xmax to the higher value
		if(p1.x < p2.x)
		{
			xmin = p1.x;
			xmax = p2.x;
		}
		else
		{
			xmin = p2.x;
			xmax = p1.x;
		}

		// draw line in terms of y slope
		float slope = ydiff / xdiff;
		for(float x = xmin; x <= xmax; x += 1.0f)
		{
			float y = p1.y + ((x - p1.x) * slope);
			setPixelToColor(x, y, c);
		}
	} 
	else
	{
		float ymin, ymax;

		// set ymin to the lower y value given
		// and ymax to the higher value
		if(p1.y < p2.y)
		{
			ymin = p1.y;
			ymax = p2.y;
		}
		else
		{
			ymin = p2.y;
			ymax = p1.y;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for(float y = ymin; y <= ymax; y += 1.0f)
		{
			float x = p1.x + ((y - p1.y) * slope);
			setPixelToColor(x, y, c);
		}
	}
}


int framebuffer::_indexOfPixel(int x, int y)
{
	int pos = -1;
	int texw   = ScreenSize.w * 4;
	int yloc   = y-1;
	int xloc   = x*4;
	pos = texw * yloc + xloc;
	return pos;
}


int framebuffer::ScreenWidth()  { return ScreenSize.w; }
int framebuffer::ScreenHeight() { return ScreenSize.h; }
