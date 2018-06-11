
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
float distanceBetween(vertex v1, vertex v2);

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
	Camera->Position.z = -20.0f;

	// Set up the voxel grid
	Voxelgrid = new voxelgrid(50, 50, 50);
	// Place it somewhere for the kicks
	Voxelgrid->Position.x = 0.0f;
	Voxelgrid->Position.y = 0.0f;
	Voxelgrid->Position.z = 0.0f;

	// For FPS tracking
	fps_start = SDL_GetTicks();
	fps_then = SDL_GetTicks();

	// Load a font
	Font = new font();
	Font->loadFont("font/NotoSans20.voxelfont");

	// Main render loop
	bool render=true;
	int debugfps = 0;
	
	// Camera movement speed
	float camspeed = 0.2f;
	// Camera rotation speed
	float camrot = 0.025f;

	while (render==true)
	{
		FB->clear();

		if (showVoxelgridOutline == true)
		{ drawVoxelgridOutline(Voxelgrid, FB, Camera, FB->FOV);  }
		
		std::string fps_str = "FPS: ";
		fps_str.append(std::to_string(debugfps));
		writeString(fps_str, Font, FB, color(255, 255, 255, 255), pixel(50, 50));

		std::string dist_str = "Distance: ";
		dist_str.append(std::to_string(distanceBetween(Voxelgrid->Position, Camera->Position)));
		dist_str.append(" m");
		writeString(dist_str, Font, FB, color(255, 255, 255, 255), pixel(50, 80));
		

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
		if (Events->KeyPressed == SDLK_w) { Camera->moveForward(camspeed); }
		if (Events->KeyPressed == SDLK_s) { Camera->moveBackward(camspeed); }
		if (Events->KeyPressed == SDLK_a) { Camera->moveLeft(camspeed); }
		if (Events->KeyPressed == SDLK_d) { Camera->moveRight(camspeed); }
		if (Events->KeyPressed == SDLK_q) { Camera->moveUp(camspeed); }
		if (Events->KeyPressed == SDLK_e) { Camera->moveDown(camspeed); }

		// Rotation with arrows
		if (Events->KeyPressed == SDLK_RIGHT) { Camera->rotateRight(camrot); }
		if (Events->KeyPressed == SDLK_LEFT)  { Camera->rotateLeft(camrot); }
		if (Events->KeyPressed == SDLK_UP)    { Camera->rotateUp(camrot); }
		if (Events->KeyPressed == SDLK_DOWN)  { Camera->rotateDown(camrot); }

		//SDL_Delay(16);
	}

	// Terminate
	SDL_DestroyWindow(Display->MainWindow);
	SDL_Quit();

	// return the 0
	return 0;
}
