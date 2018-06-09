
#include "camera.h"

camera::camera()
{ Up = vertex(0.0f, 1.0f, 0.0f); Forward = vertex(0.0f, 0.0f, -1.0f); }

void camera::moveLeft(float f)  	{ Position.x = Position.x - f; }
void camera::moveRight(float f) 	{ Position.x = Position.x + f; }
void camera::moveUp(float f) 		{ Position.y = Position.y + f; }
void camera::moveDown(float f) 		{ Position.y = Position.y - f; }
void camera::moveForward(float f) 	{ Position.z = Position.z + f; }
void camera::moveBackward(float f) 	{ Position.z = Position.z - f; }

