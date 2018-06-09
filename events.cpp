
#include "events.h"

events::events()
{ shouldQuit = false; }

void events::handleEvent()
{
	SDL_PollEvent(&Event);

	if (Event.type == SDL_QUIT)
	{ shouldQuit = true; }

	// Fast check for keys
	if (Event.type == SDL_KEYDOWN)
	{ KeyPressed = Event.key.keysym.sym; }
	if (Event.type == SDL_KEYUP)
	{ KeyPressed = -1; }
}
