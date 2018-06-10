// A simple class to read and store a font we can use to display any text of any
// font we have converted into the format we need.

#ifndef _VOXEL_FONT_
#define _VOXEL_FONT_

#include <cstdlib>
#include <string>
#include <vector>
#include "fileio.h"
#include "fontdata.h"

class font
{
public:
	// This font's name:
	std::string FontName;

	// This font's size
	int FontSize;

	// The font character data
	std::vector<fontdata> FontData;

	// Loads up a font
	void loadFont(std::string filename);
};

#endif
