#include "bin.h"
#include <iostream>

Bin::Bin()
{
	binId = 0;
	depth = 0;
	currentSize = 0;

	binContents = new game[depth];
}

Bin::Bin(int id, int d)
{
	binId = id;
	depth = d;
	currentSize = 0;

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

int Bin::getCurrentSize()
{
	return currentSize;
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

bool Bin::isFull()
{
	return currentSize == depth;
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

game* Bin::getTopGame()
{
	return &binContents[binStack.top()];
}


void Bin::pushGame(game newGame)
{
	if (currentSize < depth)
	{
		binStack.push(currentSize);
		binContents[currentSize] = newGame;

		currentSize++;
	}
}


game Bin::popGame()
{
	int index = binStack.top();
	game newGame = game();

	newGame.name = binContents[index].name;
	newGame.rarity = binContents[index].rarity;
	newGame.storeBuyPrice = binContents[index].storeBuyPrice;
	newGame.storeSellPrice = binContents[index].storeSellPrice;

	binStack.pop();
	binContents[index] = game();

	currentSize--;

	return newGame;
}