
// This sets up and maintains the window and rendering context. I won't comment
// on the functions and variables... look up the SDL2 Wiki for that. This is
// just a sandbox.

#ifndef _VOXEL_DISPLAY_
#define _VOXEL_DISPLAY_

#include <cstdlib>
#include <SDL2/SDL.h>
#include "framebuffer.h"

class display
{
public:
	SDL_Window 		*MainWindow;
	SDL_Renderer 	*Renderer;
	SDL_Texture 	*Texture;
	SDL_Surface 	*Surface;

	// Sets up the display
	display(int w, int h);

	// Spits out the final image to screen
	void flip(framebuffer *fb);
};

#endif
