
#include "voxelgrid.h"

voxelgrid::voxelgrid(int s)
{
	// Define size
	GridSize = s;
	VoxelAmount = s*s*s;
	ModelOpenedFromFile = NO;
}


voxelgrid::voxelgrid(std::string filename)
{
	// This came from a file
	ModelOpenedFromFile = YES;

	// We'll open the pipe here, but we can't close it until the model is
	// removed entirely from memory.
    FileReader.openFileForReading(filename);

    // We first make sure that this is a valid file for us... we first read
    // the header string definition, and the file version.
    std::string grid_hdr = FileReader.readString();
    float grid_ver = FileReader.readFloat();

    // If the header string is what we want and the version is okay, we can
    // proceed loading the file.
    if (grid_hdr.compare("LASTGL_MODEL") == 0 && grid_ver >= 1.0f)
	{
		// OK - First read the size of the voxelgrid in all directions
		GridSize = FileReader.readInt();
		// And determine total possible voxel amount
		VoxelAmount = GridSize * GridSize * GridSize;

		// Next up is total amount of nodes in this model, no matter at which
		// level they are.
		NodesAmount = FileReader.readInt();

		// Then we need to know the total amount of colors in this octree model
		int amount_colors = FileReader.readInt();

		// Read in amount of colors
		for (int i=0; i<amount_colors; i++)
		{
			int r = FileReader.readInt();
			int g = FileReader.readInt();
			int b = FileReader.readInt();
			color c(r, g, b);
			Colors.push_back(c);
		}

		// Now we walk through the single nodes
		for (int i=0; i<NodesAmount; i++)
		{
			// Get level of node
			int level = FileReader.readInt();

			// Size of this node level in every direction
			float levelsize = FileReader.readFloat();
			NodeLevelSizes.push_back(levelsize);

			// Now note the found level
			NodeLevels.push_back(level);

			// How many nodes are there in this level?
			int amount_nodes = FileReader.readInt();
			NodeLevelNodes.push_back(amount_nodes);

			// And finally, the position in the file containing the node info
			// of this level
			int position = FileReader.CurrentStreamPosition();
			NodeLevelPositions.push_back(position);

			// And so that we can find the remaining information, we need to
			// walk through the file by simulating reading that file. Since we
			// run on x64 systems, we do know the data type sizes and can
			// calculate a "far jump" in the file.
			// How to get the 16:
			// A vertex has three floats, so that's 12 bytes. Plus an int for
			// the color, another 4 bytes. 12 + 4 = 16. So we get 16 bytes
			// per node (and thus, ultimately, a voxel).
            int far_jump = 16 * amount_nodes;
            FileReader.jumpToPositionInFile(position + far_jump);
		}

		// Jump to somewhere in file not necessary, function ends
	}
	else
	{
		// If those two do not match, this isn't read from file and we can close
		// the file pipe.
		ModelOpenedFromFile = NO;
		FileReader.closeFileForReading();
	}
}


voxelgrid::~voxelgrid()
{
	Colors.clear();
	Nodes.clear();
	if (ModelOpenedFromFile == YES)
	{ FileReader.closeFileForReading(); }
}


float voxelgrid::getRadius()
{
	// The radius
	float r = ((float)GridSize / 2) * VOXELSIZE;
	return r;
}


int voxelgrid::getNodeLevelPosition(int level)
{
	int p = -1;
	for (int i=0; i<NodeLevels.size(); i++)
	{ if (NodeLevels[i] == level) { p = i; break; } }
	return p;
}


void voxelgrid::getNodesForLevel(int level)
{
	// Clear nodes
    Nodes.clear();

	// Find exact position of nodes in file
	int p = getNodeLevelPosition(level);

	// Get number of nodes
	int n = NodeLevelNodes[p];

	// Jump to file position
	FileReader.jumpToPositionInFile(NodeLevelPositions[p]);

	// Get info
	for (int i=0; i<n; i++)
	{
		float x, y, z;
		int c;
		x = FileReader.readFloat();
		y = FileReader.readFloat();
		z = FileReader.readFloat();
		c = FileReader.readInt();
		voxelgridnode gn(vertex(x,y,z), c);
		Nodes.push_back(gn);
	}
}
