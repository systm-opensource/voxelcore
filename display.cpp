
#include "display.h"

display::display(int w, int h)
{
	// Init SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Set up a window
	MainWindow = SDL_CreateWindow("Voxel Core", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);

	// Set up the window surface
	Surface = SDL_GetWindowSurface(MainWindow);
	//Surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
	
	// Set up renderer
	Renderer = SDL_CreateSoftwareRenderer(Surface);

	// Create renderer
	//Renderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);

	// Create the texture
	//Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}


void display::flip(framebuffer *fb)
{
	//SDL_SetRenderDrawColor(Renderer, fb->BackgroundColor.r, fb->BackgroundColor.g, fb->BackgroundColor.b, 255);
	//SDL_RenderClear(Renderer);

	//SDL_LockSurface(Surface);
	memcpy(Surface->pixels, fb->Pixels, fb->ScreenWidth() * fb->ScreenHeight() * 4);
	//SDL_UnlockSurface(Surface);
	SDL_UpdateWindowSurface(MainWindow);
	//SDL_UpdateTexture(Texture, NULL, fb->Pixels, fb->ScreenWidth() * 4);
	//SDL_RenderCopy(Renderer, Texture, NULL, NULL);
	//SDL_RenderPresent(Renderer);
}
