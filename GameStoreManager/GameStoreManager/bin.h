#ifndef BIN_H
#define BIN_H

#include<string>

using namespace std;

//Enums
enum gameRarity { COMMON, UNCOMMON, SEMI_RARE, RARE };

//Structs
struct game
{
	string name;
	gameRarity rarity;
	double storeBuyPrice;
	double storeSellPrice;
};

/**


**/
class Bin
{
	private:
		int binId;
		int depth;
		game* binContents;
		//STACK DEFINITION HERE

	public:
		Bin();
		Bin(int, int);

		int getId();
		int getDepth();
		bool gameExists(string);
		game* getGame(string);
		game* getGame(int);

};

#endif