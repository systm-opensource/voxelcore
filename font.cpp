
#include <font.h>

void font::loadFont(std::string filename)
{
	fileio reader;
	reader.openFileForReading(filename);

	// Header
	std::string hdr = reader.readString();
	// Version
	float ver = reader.readFloat();

	// QSFONT in header and at least version 1.0
	if (hdr.compare("QSFONT") == 0 && ver >= 1.0f)
	{
		FontName = reader.readString();
		FontSize = reader.readInt();

		// Number of characters to expect
		int numChars = reader.readInt();

		int i;
		for (i=1; i<=numChars; i++)
		{
			// New object
			fontdata fd;

			// Begin to read the font data

			// The ASCII
			int thisASCII = reader.readInt();
			fd.ASCIICode = thisASCII;

			// this character's width
			fd.CharacterWidth = reader.readInt();

			// Pixels to expect
			int numpix = reader.readInt();
			int j;
			for (j=1; j<=numpix; j++)
			{
				int xpos    = reader.readInt();
				int ypos    = reader.readInt();
				float alpha = reader.readFloat();
				fd.X.push_back(xpos);
				fd.Y.push_back(ypos);				
				fd.PixelAlpha.push_back(alpha);
			}

			// Add the info 
			FontData.push_back(fd);
		}

		// Create the space character
		fontdata space;
		space.ASCIICode = 32;
		space.CharacterWidth = reader.readInt();
		FontData.push_back(space);
		// This positions the ASCII 32 always at the end of the array

		// Close the pipe
		reader.closeFileForReading();
	}
}
