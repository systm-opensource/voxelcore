
// This file monitors and manages events such as mouse clicks, drags, scrolls,
// as well as keyboard interaction.

#ifndef _VOXEL_EVENTS_
#define _VOXEL_EVENTS_

#include <cstdlib>
#include <SDL2/SDL.h>

class events
{
public:
	// Should we quit?
	bool shouldQuit;

	// Tbe event that just happened
	SDL_Event Event;

	// The key pressed
	int KeyPressed;

	// Empty constructor
	events();

	// Handles the event
	void handleEvent();
};

#endif
