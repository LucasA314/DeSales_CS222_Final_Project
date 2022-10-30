#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<cmath>
#include<ctime>

#include "bin.h"

using namespace std;

//Constants
const int MAX_HIGH_SCORES = 10;
const int TOTAL_BINS = 3;
const int BIN_DEPTH = 5;
const int TOTAL_DAYS = 5;
const int START_HOUR = 8;
const int MINUTES_PER_HOUR = 60;
const double TOTAL_HOURS = 9.0;
const double STARTING_MONEY = 100.0;
const double COMMON_BUY_PRICE = 5.0;
const double UNCOMMON_BUY_PRICE = 10.0;
const double SEMI_RARE_BUY_PRICE = 15.0;
const double RARE_BUY_PRICE = 20.0;
const double GENERAL_SELL_MARGIN = 1.5;
const double RARE_SELL_MARGIN = 2.0;

//Enumerations
enum titleMenuChoices { PLAY, HOW_TO_PLAY, VIEW_HIGH_SCORES, QUIT };
enum customerChoices { SELL, BUY, NO_DEAL };

//Structs
struct customer
{
	string firstName;
	string lastName;
	game sellGame;
	game buyGame;
};


//Function Headers
titleMenuChoices titleScreen();
void howToPlay();
void viewHighScores();
void playGame();
game* loadGameInformation(int&);
customer createCustomer(game*, int);
void displayTurnInfo(double, double, customer);
void timeRemainingToTimeOfDay(double, int&, int&);
customerChoices makeCustomerChoice(Bin[], int, string);
bool gameInStock(Bin[], int, string);
string rarityText(gameRarity);
void sellGame(Bin[], int, game&, double&, double&); //NEEDS TO BE FINISHED
void buyGame(Bin[], int, game&, double&, double&); //NEEDS TO BE FINISHED
void noDeal(string, string, double&);
void displayEndOfDayReport(); //NEEDS TO BE WRITTEN
void displayGameOverReport(); //NEEDS TO BE WRITTEN
void updateHighScores(int); //NEEDS TO BE WRITTEN
void displayBins(Bin[], int);


int main()
{
	bool readHowToPlay = false;
	titleMenuChoices menuChoice;

	//Set a Random Seed
	srand(time(0));

	//Title Menu Loop
	do
	{
		menuChoice = titleScreen();

		switch (menuChoice)
		{
			case PLAY:
				if (!readHowToPlay)
				{
					howToPlay();
					readHowToPlay = true;
				}

				playGame();
				break;
			case HOW_TO_PLAY:
				howToPlay();
				readHowToPlay = true;
				break;
			case VIEW_HIGH_SCORES:
				viewHighScores();
				break;
		}

	} while (menuChoice != QUIT);

	//Display the Closing Message
	std::cout << "\n\n";
	std::cout << "Thank you for Playing!\n";
	std::cout << "See you again soon!\n";

	return 0;
}

/**

**/
titleMenuChoices titleScreen()
{
	int choice;
	titleMenuChoices eChoice;

	//Display the Title
	std::cout << "*------------------------*\n";
	std::cout << "|   GAME STORE MANAGER   |\n";
	std::cout << "*------------------------*\n\n";

	do
	{
		//Display the Menu Options
		std::cout << "1) Start Game\n";
		std::cout << "2) How to Play\n";
		std::cout << "3) View High Scores\n";
		std::cout << "4) Quit\n\n";
		std::cout << "Please Enter a Selection: ";
		std::cin >> choice;

		//Input Validation
		if (choice != 1 && choice != 2 && choice != 3 && choice != 4)
		{
			std::cout << "\nInvalid Choice: Please enter a number 1 - 4.\n\n";
		}

	} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);

	//Convert to Enum Type
	switch (choice)
	{
		case 1:
			eChoice = PLAY;
			break;
		case 2:
			eChoice = HOW_TO_PLAY;
			break;
		case 3:
			eChoice = VIEW_HIGH_SCORES;
			break;
		case 4:
			eChoice = QUIT;
			break;
	}

	return eChoice;
}

/**

**/
void howToPlay()
{
	char input;

	//Display Information
	std::cout << "*-------------*\n";
	std::cout << "| HOW TO PLAY |\n";
	std::cout << "*-------------*\n";

	std::cout << "Story: Congratulations! You are officialy the owner of your very own\n";
	std::cout << "       used video game store. Now it is time to open for business.\n";
	std::cout << "       Store games in your bins efficiently and help customers buy and\n";
	std::cout << "       sell games to maximize your profit each day. How much can you earn\n";
	std::cout << "       during your first week?\n";

	do
	{
		//Prompt Return to Main Menu
		std::cout << "\n\n";
		std::cout << "Type c and press enter to continue: ";
		std::cin >> input;

		//Input Validation
		if (tolower(input) != 'c')
		{
			std::cout << "\nInvalid Input: Please enter the character c to continue.\n\n";
		}
	} while (tolower(input) != 'c');
}

/**

**/
void viewHighScores()
{
	ifstream scoresIn;
	char input;

	//Display Information
	std::cout << "*-----------------*\n";
	std::cout << "|   HIGH SCORES   |\n";
	std::cout << "*-----------------*\n";
	std::cout << "| Name |    Score |\n";

	//Read in High Scores
	scoresIn.open("scores.dat");

	for (int i = 0; i < MAX_HIGH_SCORES; i++)
	{
		string name;
		int score;

		if (scoresIn >> name)
		{
			scoresIn >> score;

			cout << "| " << setw(4) << name << " | " << right << setw(8) << score << " |\n";
		}
		else
		{
			cout << "| ???? | 00000000 |\n";
		}
	}
	cout << "*-----------------*\n\n";

	scoresIn.close();

	do
	{
		//Prompt Return to Main Menu
		std::cout << "\n\n";
		std::cout << "Type c and press enter to continue: ";
		std::cin >> input;

		//Input Validation
		if (tolower(input) != 'c')
		{
			std::cout << "\nInvalid Input: Please enter the character c to continue.\n\n";
		}
	} while (tolower(input) != 'c');
}

/*

*/
void playGame()
{
	double hoursRemaining = TOTAL_HOURS;
	double money = STARTING_MONEY;
	int totalGames = 0;
	customerChoices custChoice;
	Bin bins[TOTAL_BINS];

	//Load All the Game Information
	game* gameList = loadGameInformation(totalGames);

	//Set Up the Bins
	for (int i = 0; i < TOTAL_BINS; i++)
	{
		bins[i] = Bin(i, BIN_DEPTH);
	}

	//Run Each Day
	for (int day = 1; day <= TOTAL_DAYS; day++)
	{
		std::cout << "*-------*\n";
		std::cout << "| DAY " << day << " | \n";
		std::cout << "*-------*\n";

		while (hoursRemaining > 0)
		{
			//Create a Customer
			customer cust = createCustomer(gameList, totalGames);

			//Display Current Turn Info
			displayTurnInfo(hoursRemaining, money, cust);

			//Make a Choice
			custChoice = makeCustomerChoice(bins, TOTAL_BINS, cust.sellGame.name);

			//Take the Action
			switch (custChoice)
			{
				case SELL:
					sellGame(bins, TOTAL_BINS, cust.sellGame, hoursRemaining, money);
					break;
				case BUY:
					buyGame(bins, TOTAL_BINS, cust.buyGame, hoursRemaining, money);
					break;
				case NO_DEAL:
					noDeal(cust.firstName, cust.lastName, hoursRemaining);
					break;
			}
		}

		//Display the End of Day Report
		displayEndOfDayReport();

		//Reset the Timer
		hoursRemaining = TOTAL_HOURS;
	}

	//Display Game Over Report
	displayGameOverReport();

	//Check for High Score
	updateHighScores(max(0.0, floor(money - STARTING_MONEY)));

	//Display High Scores
	viewHighScores();
}

/*

*/
game* loadGameInformation(int &totalEntries)
{
	ifstream dataIn;
	int rarity;

	//Open the File
	dataIn.open("gameInfo.dat");

	//Read the File Length
	dataIn >> totalEntries;
	dataIn.ignore();

	//Set the Array Size
	game* gameList = new game[totalEntries];

	//Loop Through the Data
	for (int i = 0; i < totalEntries; i++)
	{
		//Read In Each Entry
		getline(dataIn, gameList[i].name);

		dataIn >> rarity;
		dataIn.ignore();

		switch (rarity)
		{
			case 1:
				gameList[i].rarity = COMMON;
				gameList[i].storeBuyPrice = COMMON_BUY_PRICE;
				gameList[i].storeSellPrice = COMMON_BUY_PRICE * GENERAL_SELL_MARGIN;
				break;
			case 2:
				gameList[i].rarity = UNCOMMON;
				gameList[i].storeBuyPrice = UNCOMMON_BUY_PRICE;
				gameList[i].storeSellPrice = UNCOMMON_BUY_PRICE * GENERAL_SELL_MARGIN;
				break;
			case 3:
				gameList[i].rarity = SEMI_RARE;
				gameList[i].storeBuyPrice = SEMI_RARE_BUY_PRICE;
				gameList[i].storeSellPrice = SEMI_RARE_BUY_PRICE * GENERAL_SELL_MARGIN;
				break;
			case 4:
				gameList[i].rarity = RARE;
				gameList[i].storeBuyPrice = RARE_BUY_PRICE;
				gameList[i].storeSellPrice = RARE_BUY_PRICE * RARE_SELL_MARGIN;
				break;
		}
	}

	//Close the File
	dataIn.close();

	return gameList;
}


/**

**/
customer createCustomer(game* gameInfo, int totalGames)
{
	customer cust;
	int randInt;

	//Set First Names
	string firstNames[] = {"Todd", "Angela", "Priscilla", "Luke", "Carl", "Caitlin"};
	string lastNames[] = {"Baker", "Smith", "Cossman", "Hammarsten", "Stevens"};

	//Set the Customer Information
	cust.firstName = firstNames[rand() % (sizeof(firstNames)/sizeof(string))];
	cust.lastName = lastNames[rand() % (sizeof(lastNames)/sizeof(string))];
	cust.sellGame = gameInfo[rand() % totalGames];
	cust.buyGame = gameInfo[rand() % totalGames];


	return cust;
}


/**

**/
void displayTurnInfo(double hours, double money, customer cust)
{
	int hour = 0;
	int minute = 0;

	timeRemainingToTimeOfDay(hours, hour, minute);

	//Diplay the Time
	std::cout << "Time: " << hour << ":";
	if (minute < 10)
	{
		std::cout << "0";
	}
	std::cout << minute << endl;

	//Display the Current Amount
	std::cout << fixed << showpoint << setprecision(2);
	std::cout << "Store Balance: $" << money << endl;
	std::cout << endl;

	//Display the Customer Info
	std::cout << "Current Customer: " << cust.firstName << " " << cust.lastName << endl;
	std::cout << "Looking to sell " << cust.sellGame.name << "(" << rarityText(cust.sellGame.rarity) << ")" << " for $" << cust.sellGame.storeBuyPrice << endl;
	std::cout << "Looking to buy " << cust.buyGame.name << "(" << rarityText(cust.buyGame.rarity) << ")" << " for $" << cust.buyGame.storeSellPrice << endl;
	std::cout << endl;
}

/**

**/
void timeRemainingToTimeOfDay(double timeRemaining, int& hour, int& minute)
{
	//Calculate How Much Time Has Passed
	double timePassed = TOTAL_HOURS - timeRemaining;

	//Convert the Numbers
	hour = START_HOUR + floor(timePassed);
	minute = static_cast<int>((timePassed - floor(timePassed)) * MINUTES_PER_HOUR);
}


/**

**/
customerChoices makeCustomerChoice(Bin bins[], int s, string sellGame)
{
	int choice;
	customerChoices eChoice;

	do
	{
		//Display the Menu Options
		std::cout << "1) Buy Customer's Game\n";
		std::cout << "2) Sell Customer's Desired Game\n";
		std::cout << "3) Offer No Deal to the Customer\n\n";
		std::cout << "Please Enter a Selection: ";
		std::cin >> choice;

		//Input Validation
		if (choice != 1 && choice != 2 && choice != 3)
		{
			std::cout << "\nInvalid Choice: Please enter a number 1 - 3.\n\n";
		}
		else if (choice == 2 && !gameInStock(bins, s, sellGame))
		{
			std::cout << "You do not have the customer's desired game in stock. Please make a different selection.\n\n";
		}

	} while (choice != 1 && choice != 2 && choice != 3);

	//Convert to Enum Type
	switch (choice)
	{
		case 1:
			eChoice = SELL;
			break;
		case 2:
			eChoice = BUY;
			break;
		case 3:
			eChoice = NO_DEAL;
			break;
	}

	return eChoice;
}

/**

**/
bool gameInStock(Bin bins[], int s, string n)
{
	//Check Each Bin
	for (int i = 0; i < s; i++)
	{
		//Check if the Game Is In the Bin
		if (bins[i].gameExists(n))
		{
			return true;
		}
	}
	
	return false;
}

/*

*/
string rarityText(gameRarity rarity)
{
	switch (rarity)
	{
		case COMMON:
			return "Common";
			break;
		case UNCOMMON:
			return "Uncommon";
			break;
		case SEMI_RARE:
			return "Semi-Rare";
			break;
		case RARE:
			return "Rare";
			break;
	}
}

/**

**/
void sellGame(Bin bins[], int size, game &g, double &remainingHours, double &money)
{
	bool gameInHand = true;
	game* inHand = &g;

	cout << "Select the Bin in which to place " << g.name << ".\n";

	//Display the Bins
	displayBins(bins, size);

	//Prompt the User to Move Games Around

	//Add the Purchase Amount
	money += g.storeBuyPrice;
}

/**

**/
void buyGame(Bin bins[], int size, game &g, double& remainingHours, double& money)
{
	bool gameInHand = false;
	game* inHand = nullptr;

	cout << "Select " << g.name << " from the storage bins: ";

	//Display the Bins
	displayBins(bins, size);

	//Prompt the User to Move Games Around

	//Subtract the Payment Amount
	money -= g.storeBuyPrice;
}

/**

**/
void noDeal(string firstName, string lastName, double &remainingHours)
{
	cout << "You turned " << firstName << " " << lastName << " away.\n";

	remainingHours -= 0.5;
}

/**

**/
void displayEndOfDayReport()
{

}

/**

**/
void displayGameOverReport()
{

}

/**

**/
void updateHighScores(int score)
{


}

/**

**/
void displayBins(Bin bins[], int size)
{
	//Display the Top
	for (int b = 0; b < size; b++)
	{
		cout << "*     *  ";
	}
	cout << endl;

	for (int i = 0; i < bins[0].getDepth(); i++)
	{
		string game;

		//Get the First Five Letters of the Game Name
		for (int b = 0; b < size; b++)
		{
			//Check if a Game Exists
			if (bins[b].getGame(i) == nullptr)
			{
				game = "     ";
			}
			else
			{
				game = bins[b].getGame(i)->name.substr(0, 5);
			}

			//Display the Name
			cout << "|" << setw(5) << game << "|  ";
		}
		cout << endl;
	}

	//Display the Bottom
	for (int b = 0; b < size; b++)
	{
		cout << "*-----*  ";
	}
	cout << endl;
}