
// This is the main function of the Voxel Core test program.

#include <cstdlib>
#include <iostream>
#include <SDL2/SDL.h>
#include "camera.h"
#include "display.h"
#include "events.h"
#include "framebuffer.h"
#include "voxelgrid.h"
#include "font.h"

// Forward...
void drawVoxelgridOutline(voxelgrid *g, framebuffer *b, camera *cm, int fov);
void writeString(std::string t, font *f, framebuffer *fb, color c, pixel p);

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

// A font to render text
font *Font;

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
	Voxelgrid->Position.z = 20.0f;

	// For FPS tracking
	fps_start = SDL_GetTicks();
	fps_then = SDL_GetTicks();

	// Load a font
	Font = new font();
	Font->loadFont("font/NotoSans20.voxelfont");

	// Main render loop
	bool render=true;
	int debugfps = 0;

	while (render==true)
	{
		FB->clear();

		if (showVoxelgridOutline == true)
		{ drawVoxelgridOutline(Voxelgrid, FB, Camera, FB->FOV);  }
		
		writeString("FPS: ", Font, FB, color(255, 255, 255, 255), pixel(50, 50));
		writeString(std::to_string(debugfps), Font, FB, color(255, 255, 255, 255), pixel(120, 50));

		//FB->clear();
		Display->flip(FB);

		// One cycle done
		fps++;

		fps_start = SDL_GetTicks();
		
		if (fps_start - fps_then >= 1000)
		{ debugfps = fps; fps = 0; fps_then = fps_start; }

		// Check events
		Events->handleEvent();
		if (Events->shouldQuit == true)
		{ render=false; }

		// WASD-Keys for the camera
		float camspeed = 0.2f;
		if (Events->KeyPressed == SDLK_w) { Camera->moveForward(camspeed); }
		if (Events->KeyPressed == SDLK_s) { Camera->moveBackward(camspeed); }
		if (Events->KeyPressed == SDLK_a) { Camera->moveLeft(camspeed); }
		if (Events->KeyPressed == SDLK_d) { Camera->moveRight(camspeed); }
		if (Events->KeyPressed == SDLK_q) { Camera->moveUp(camspeed); }
		if (Events->KeyPressed == SDLK_e) { Camera->moveDown(camspeed); }

		

		//SDL_Delay(16);
	}

	// Terminate
	SDL_DestroyWindow(Display->MainWindow);
	SDL_Quit();

	// return the 0
	return 0;
}
