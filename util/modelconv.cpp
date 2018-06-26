
/*
--------------------------------------------------------------------------------
 last.GL: The Model to Voxel converter (util/modelconv)
--------------------------------------------------------------------------------
 Takes in an Wavefront OBJ model file, and converts into the octree file
 format we need. When everything is done, the file is compressed using lz.
 
 Build program with the build.sh script which is in the same directory as this
 source file.
 
 Run with ./modelconv /path/to/obj-file

 Developed by M. Strehlow - <mstr@syst-m.info>
 >> www.syst-m.gl/last
--------------------------------------------------------------------------------
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/


// -----------------------------------------------------------------------------
// Some important things we need.
// -----------------------------------------------------------------------------
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <zlib.h>


// -----------------------------------------------------------------------------
// We will use some things from voxelcore, to make life easier a bit.
// -----------------------------------------------------------------------------
#include "fileio.h"
#include "color.h"
#include "vertex.h"
#include "voxelgridnode.h"


// -----------------------------------------------------------------------------
// A small class that describes a triangle found in the source OBJ file.
// -----------------------------------------------------------------------------
class triangle
{
public:
	int v1;
	int v2;
	int v3;
};
// Note: this class does not appear in Voxelcore and last.GL.


// -----------------------------------------------------------------------------
// Things used in some or all functions
// -----------------------------------------------------------------------------
std::vector<vertex> 		Vertices;
std::vector<vertex>			Voxels;
std::vector<triangle>		Triangles;
std::vector<color>			Colors;
std::vector<voxelgridnode> 	OctreeNodes; 		  // This is the important stuff
fileio 						FileWriter;


// -----------------------------------------------------------------------------
// Print out a generic header when the tool is run
// -----------------------------------------------------------------------------
void printHeader()
{
	std::cout << "\n";
	std::cout << " last.GL: Model to Voxel converter - ModelConv\n";
	std::cout << " Developed by M. Strehlow <mstr@syst-m.info>\n";
	std::cout << " >> www.syst-m.info/last\n";
	std::cout << "\n";
	std::cout << " This tool is part of the last.GL package and is therefore free and open\n";
	std::cout << " source software.\n";
	std::cout << "\n";
}


// -----------------------------------------------------------------------------
// Print out the usage.
// -----------------------------------------------------------------------------
void printUsage()
{
	std::cout << " USAGE:\n";
	std::cout << " ./modelconv OBJ-MODEL DESTINATION-FILE\n";
	std::cout << "\n";
	std::cout << " The path of source and destination can be absolute or relative.\n";
	std::cout << "\n";
}


// -----------------------------------------------------------------------------
// Read the vertices from the OBJ file, and place them in the array.
// Make sure no vertex appears twice.
// -----------------------------------------------------------------------------
void getVerticesFromSourceFile(std::string source)
{
	// In this case we have to open the file, read it line by line, and find all
	// lines that start with a "v". Then split that line into its segments and
	// extract the vertex coordinates
	std::ifstream f(source);	// The stream
	std::string l;				// The string for each line
	std::string d = " "; 		// Space is the delimiter for each line
	while (std::getline(f, l))
	{
		// Use a trick to split each line by space
		size_t pos = 0;
		std::string token;
		std::vector<std::string> parts;
		while ((pos = l.find(d)) != std::string::npos)
		{
			token = l.substr(0, pos);
			parts.push_back(token);
			//std::cout << token << std::endl;
			l.erase(0, pos + d.length());
		}
		
		if (parts[0].compare("v") == 0)
		{
			// Create a vertex
			float x, y, z;
			x = std::atof(parts[1].c_str());
			y = std::atof(parts[2].c_str());
			z = std::atof(parts[3].c_str());
			vertex v(x,y,z);
			Vertices.push_back(v);
		}
	}
	
	// Close the file
	f.close();
	
	// How many did we find?
	std::cout << " [I] Got " << Vertices.size() << " vertices from model file.\n";
}


// -----------------------------------------------------------------------------
// Here we find out which vertex belongs to which face, and store that data
// temporarily. With that information we can then begin to voxelize each face,
// and then, in turn, create the octree information.
// -----------------------------------------------------------------------------
void getFaceIndicesFromSourceFile(std::string source)
{
	// Same as above, but this time we are looking for the face vertex indices.
	std::ifstream f(source);	// The stream
	std::string l;				// The string for each line
	std::string d = " "; 		// Space is the delimiter for each line
	while (std::getline(f, l))
	{
		// Use a trick to split each line by space
		size_t pos = 0;
		std::string token;
		std::vector<std::string> parts;
		while ((pos = l.find(d)) != std::string::npos)
		{
			token = l.substr(0, pos);
			parts.push_back(token);
			l.erase(0, pos + d.length());
		}
		
		if (parts[0].compare("f") == 0)
		{
			// This is face index information.
			int v1 = std::atoi(parts[1].c_str());
			int v2 = std::atoi(parts[2].c_str());
			int v3 = std::atoi(parts[3].c_str());
			triangle t;
			t.v1 = v1; t.v2 = v2; t.v3 = v3;
			Triangles.push_back(t);
		}
	}
	
	// Close the file
	f.close();
	
	// Tell us what you found
	std::cout << " [I] Found " << Triangles.size() << " triangles.\n";
}


// -----------------------------------------------------------------------------
// With this call, we generate the octree data. A rather complex call. If you
// want to understand how this model conversion works, be sure to read every
// comment and inner workings of this function.
// -----------------------------------------------------------------------------
void generateOctreeData(std::string dest)
{
	// So... we first need to know the diameter of the octree grid.
}


// -----------------------------------------------------------------------------
// This is where the magic happens: convert the model
// -----------------------------------------------------------------------------
void convertModel(std::string pathtomodel, std::string destination)
{
	// Check if the file exists...
	bool exists = false;
	fileio t;
	exists = t.doesFileExist(pathtomodel);
	
	// Proceed if it exists. Exit if not.
	if (exists == true)
	{
		// Info on conversion
		std::cout << " [I] Opening " << pathtomodel << " for conversion.\n";
		
		// Begin to read out all vertices of provided OBJ file
		std::cout << " [I] Acquiring vertices.\n";
		getVerticesFromSourceFile(pathtomodel);
		
		// Get triangles
		std::cout << " [I] Acquiring triangles.\n";
		getFaceIndicesFromSourceFile(pathtomodel);
		
		// Generate the octree data
		std::cout << " [I] Creating octree node information.\n";
		generateOctreeData(std::string destination);
		
		// Display a summary of the data generated.
		
		// Info about closing the file
		std::cout << " [I] Closing source file.\n";
	}
	else
	{
		// Print an error, exit
		std::cout << " [E] File " << pathtomodel << " does not exist. Exiting.\n";
		return;
	}
}


int main(int argc, char *argv[])
{
	// Always print header
	printHeader();

	// OK - so first check if we have an argument
	if (argc == 1 || argc == 2)
	{ printUsage(); return 0; }
	
	// If we get here, something was supplied
	if (argc >= 3)
	{
		std::string source = argv[1];
		std::string dest   = argv[2];
		convertModel(source, dest);
	}
	
	return 0;
}
