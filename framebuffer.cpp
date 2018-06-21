
#include "framebuffer.h"

framebuffer::framebuffer(int w, int h)
{ FOV=std::min(w, h); resizeBuffer(rectangle(0,0,w,h)); CX = w/2; CY=h/2; }


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
	//memset(Pixels, 0, numberPixels*4);
	// Copy background color first
	memcpy(Pixels, BackgroundPixels, numberPixels*4);
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

		color o(Pixels[pix+2], Pixels[pix+1], Pixels[pix+0], Pixels[pix+3]);
		color n = _blendColors(o, c);
		Pixels[pix+3] = n.a;
		Pixels[pix+2] = n.r;
		Pixels[pix+1] = n.g;
		Pixels[pix+0] = n.b;
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


color framebuffer::_blendColors(color c1, color c2)
{
	color result(0,0,0,0);

	// Get float alpha of OLD color
	float old_a = (float)c1.a / 255.0f;

	// Get float alpha of NEW color
	float new_a = (float)c2.a / 255.0f;

	// Float values for old color
	float old_r = (float)c1.r / 255.0f;
	float old_g = (float)c1.g / 255.0f;
	float old_b = (float)c1.b / 255.0f;
	// Float values for new color
	float new_r = (float)c2.r / 255.0f;
	float new_g = (float)c2.g / 255.0f;
	float new_b = (float)c2.b / 255.0f;

	// Float values for blending results
	float blend_r = new_a * new_r + (1.0f - new_a) * old_r;	
	float blend_g = new_a * new_g + (1.0f - new_a) * old_g;	
	float blend_b = new_a * new_b + (1.0f - new_a) * old_b;	
	float blend_a = new_a * new_a + (1.0f - new_a) * old_a;

	// Update to correct values:
	blend_r = blend_r * 255;
	blend_g = blend_g * 255;
	blend_b = blend_b * 255;
	blend_a = blend_a * 255;

	// Now convert those directly into their int places
	result.r = blend_r;
	result.g = blend_g;
	result.b = blend_b;
	result.a = blend_a;

	// Return the shaded pixel color
	return result;
}


bool framebuffer::_pixelUsed(pixel p)
{
	bool used = false;
	int index = _indexOfPixel(p.x, p.y);
	if (
		Pixels[index+3] == 255 &&
		Pixels[index+2] == BackgroundColor.r &&
		Pixels[index+1] == BackgroundColor.g &&
		Pixels[index+0] == BackgroundColor.b
	   )
	{ used = false; }
	else
	{ used = true; }

	return used;
}


int framebuffer::ScreenWidth()  { return ScreenSize.w; }
int framebuffer::ScreenHeight() { return ScreenSize.h; }
