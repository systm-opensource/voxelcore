
// This describes the virtual camera that can be moved around. It has functions
// to move and rotate it. Further it has a clipping distance, which means that
// beyond this point anything will be discarded from rendering. This is a
// heavily trimmed version compared to the real one in my engine.

#ifndef _VOXEL_CAMERA_
#define _VOXEL_CAMERA_

#include <cstdlib>
#include "vertex.h"

class camera
{
public:
	// The camera's position
	vertex Position;
	// The camera's rotation
	vertex Rotation;
	// The up vector
	vertex Up;
	// The forward vector
	vertex Forward;

	// Sets up the correct vectors only
	camera();

	void moveLeft(float f);
	void moveRight(float f);
	void moveUp(float f);
	void moveDown(float f);
	void moveForward(float f);
	void moveBackward(float f);
	void rotateLeft(float f);
	void rotateRight(float f);
	void rotateUp(float f);
	void rotateDown(float f);
};

#endif
