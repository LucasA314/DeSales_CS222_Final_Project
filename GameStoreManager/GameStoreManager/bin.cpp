#include "bin.h"


Bin::Bin()
{
	binId = 0;
	depth = 0;

	binContents = new game[depth];
}

Bin::Bin(int id, int d)
{
	binId = id;
	depth = d;

	binContents = new game[depth];
}

int Bin::getId()
{
	return binId;
}

int Bin::getDepth()
{
	return depth;
}

bool Bin::gameExists(string n)
{
	for (int i = 0; i < depth; i++)
	{
		if (binContents[i].name == n)
		{
			return true;
		}
	}

	return false;
}

game* Bin::getGame(string n)
{
	for (int i = 0; i < depth; i++)
	{
		if (binContents[i].name == n)
		{
			return &binContents[i];
		}
	}
}


game* Bin::getGame(int i)
{
	return &binContents[i];
}