// Holds information about one character inside a font in our format.

#ifndef _VOXEL_FONTDATA_
#define _VOXEL_FONTDATA_

#include <cstdlib>
#include <vector>

class fontdata
{
public:
	// This ASCII code
	int ASCIICode;

	// The relative pixel locations
	std::vector<int> X;
	std::vector<int> Y;

	// Width of this character
	int CharacterWidth;

	// The alpha values per pixel
	std::vector<float> PixelAlpha;
};

#endif
