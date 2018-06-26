#include "fileio.h"

bool fileio::doesFileExist(std::string filename)
{
	std::ifstream f(filename.c_str());
    return f.good();
}

void fileio::openFileForWriting(std::string fn)
{ out.open (fn, std::ofstream::binary | std::ios::out); }

void fileio::openFileForReading(std::string fn)
{ in.open (fn, std::ifstream::binary | std::ios::in); }

void fileio::jumpToPositionInFile(int seekpos)
{ in.seekg(0, in.beg+seekpos); }

void fileio::jumpToBeginningOfFile()
{ in.seekg(0, in.beg); }

void fileio::closeFileForWriting()
{ out.close(); }

void fileio::closeFileForReading()
{ in.close(); }

void fileio::writeInt(int i) { out.write ((char *)&i, sizeof(int)); }
void fileio::writeFloat(float f) { out.write ((char *)&f, sizeof(float)); }
void fileio::writeDouble(double d) { out.write ((char *)&d, sizeof(double)); }

void fileio::writeString(std::string str)
{
	// Length of string
	int len = str.length();
	// Write length
	writeInt(len);
	// Write string itself
	//out.write( str.c_str(), str.size() );
	out.write(str.c_str(), len);
}

int fileio::readInt()
{
	int val;
	in.read((char*)&val, sizeof(int));
	return val;
}

float fileio::readFloat()
{
	float val;
	in.read((char*)&val, sizeof(float));
	return val;
}

double fileio::readDouble()
{
	double val;
	in.read((char*)&val, sizeof(double));
	return val;
}

std::string fileio::readString()
{
	// Read string length:
	int len = readInt();
	// Create buffer:
	char* temp = new char[len];
	// Read string:
	in.read(temp, len);
	std::string res = temp;
	// Return it
	return res;
}


int fileio::CurrentStreamPosition()
{ return in.tellg(); }
