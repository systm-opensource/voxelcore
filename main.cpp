
// This is the main function of the Voxel Core test program.

#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>
#include "camera.h"
#include "display.h"
#include "events.h"
#include "framebuffer.h"
#include "voxelgrid.h"

// Forward...
void drawVoxelgridOutline(voxelgrid *g, framebuffer *b, camera *cm, int fov);

// Do we want to see the outline of voxelgrids?
bool showVoxelgridOutline = true;

// Some static variables
int ScreenW = 1280;
int ScreenH = 720;

// One FB
framebuffer *FB;

// The main window
display *Display;

// Something to handle events
events *Events;

// A camera!
camera *Camera;

// For now we'll use only one voxel grid
voxelgrid *Voxelgrid;

// For FPS
Uint32 fps_start;
Uint32 fps_then;
int fps;

// The main loop
int main(int argc, char **argv[])
{
	FB = new framebuffer(ScreenW, ScreenH);
	FB->setBackgroundColor(color(33, 66, 99));
	Events = new events();
	Display = new display(ScreenW, ScreenH);
	Camera = new camera();

	// Set up the voxel grid
	Voxelgrid = new voxelgrid(50, 50, 50);
	// Place it somewhere for the kicks
	Voxelgrid->Position.x = 0.0f;
	Voxelgrid->Position.y = 0.0f;
	Voxelgrid->Position.z = 117.2f;

	// For FPS tracking
	fps_start = SDL_GetTicks();
	fps_then = SDL_GetTicks();

	// Main render loop
	bool render=true;
	while (render==true)
	{
		FB->clear();

		if (showVoxelgridOutline == true)
		{ drawVoxelgridOutline(Voxelgrid, FB, Camera, FB->FOV);  }

		//FB->clear();
		Display->flip(FB);

		// One cycle done
		fps++;

		fps_start = SDL_GetTicks();
		if (fps_start - fps_then >= 1000)
		{ std::cout << "FPS: " << fps << "\n"; fps = 0; fps_then = fps_start; }

		// Check events
		Events->handleEvent();
		if (Events->shouldQuit == true)
		{ render=false; }

		// WASD-Keys for the camera
		if (Events->KeyPressed == SDLK_w) { Camera->moveForward(2.0f); }
		if (Events->KeyPressed == SDLK_s) { Camera->moveBackward(2.0f); }
		if (Events->KeyPressed == SDLK_a) { Camera->moveLeft(2.0f); }
		if (Events->KeyPressed == SDLK_d) { Camera->moveRight(2.0f); }
		if (Events->KeyPressed == SDLK_q) { Camera->moveUp(2.0f); }
		if (Events->KeyPressed == SDLK_e) { Camera->moveDown(2.0f); }

		//SDL_Delay(16);
	}

	// Terminate
	SDL_DestroyWindow(Display->MainWindow);
	SDL_Quit();

	// return the 0
	return 0;
}
