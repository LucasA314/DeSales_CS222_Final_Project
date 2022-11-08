#ifndef BIN_H
#define BIN_H

#include<string>
#include<stack>

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
		int currentSize;
		game* binContents;
		stack<int> binStack;
		//STACK DEFINITION HERE

	public:
		Bin();
		Bin(int, int);

		int getId();
		int getDepth();
		int getCurrentSize();
		bool gameExists(string);
		bool isFull();
		game* getGame(string);
		game* getGame(int);
		game* getTopGame();
		
		void pushGame(game);
		game popGame();

};

#endif