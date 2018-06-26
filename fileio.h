
// This is a simple wrapper to read and write to binary files. This is also used
// in Reason to read all the formats that appear. For voxeltest, the font format
// is being made public. It's nothing special, really.

#ifndef _VOXEL_FILEIO_
#define _VOXEL_FILEIO_

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

class fileio
{
public:
	// The file to write out to
	std::ofstream out;
	// The file to read in from
	std::ifstream in;

	// Check if a file exists prior to opening it or writing to it
	bool doesFileExist(std::string filename);

	// Open a binary file for writing
	void openFileForWriting(std::string fn);

	// Open a binary file for reading
	void openFileForReading(std::string fn);

	// Jumps to the specified position in the file opened for reading. We mostly
	// use this to quickly jump to positions in an octree file and read only
	// what is needed for rendering.
	void jumpToPositionInFile(int seekpos);

	// Jumps back to the very beginning of the file
	void jumpToBeginningOfFile();

	// Close the file to write
	void closeFileForWriting();
	// And for reading
	void closeFileForReading();

	// Writes all kinds of things we need
	void writeInt(int i);
	void writeFloat(float f);
	void writeDouble(double d);
	void writeString(std::string str);

	// Reads back the same data types
	int readInt();
	float readFloat();
	double readDouble();
	std::string readString();

	// Get current position in file stream
	int CurrentStreamPosition();
};

#endif
