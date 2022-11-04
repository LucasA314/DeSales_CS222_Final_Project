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
void sellGame(Bin[], int, game&, double&, double&);
void buyGame(Bin[], int, game&, double&, double&);
void noDeal(Bin[], int, string, string, double&);
void displayEndOfDayReport(); //NEEDS TO BE WRITTEN
void displayGameOverReport(int, double);
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
Task: Display a Menu to allow the user to navigate the program
Input: None
Output: The choice selected as an enum
Preconditions: None
Postconditions: The choice selected is of type titleMenuChoices
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
Task: Display How to Play Information to the User
Input: None
Output: None
Preconditions: None
Postconditions: None
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
Task: Display the High Score Table to the User
Input: None
Output: None
Preconditions: A file "scores.dat" must exist and be formatted correctly
Postconditions: None
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

/**
Task: Runs the Main Game Loop
Input: None
Output: None
Preconditions: None
Postconditions: None
**/
void playGame()
{
	//Declare Variables
	double hoursRemaining = TOTAL_HOURS;
	double money = STARTING_MONEY;
	int totalGames = 0;
	int customersServed = 0;
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
			customersServed++;

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
					noDeal(bins, TOTAL_BINS, cust.firstName, cust.lastName, hoursRemaining);
					break;
			}
		}

		//Display the End of Day Report
		displayEndOfDayReport();

		//Reset the Timer
		hoursRemaining = TOTAL_HOURS;
	}

	//Display Game Over Report
	displayGameOverReport(customersServed, money);

	//Check for High Score
	updateHighScores(max(0.0, floor(money - STARTING_MONEY)));

	//Display High Scores
	viewHighScores();
}

/**
Task: Reads in Information on Each of the Games from a File
Input: totalEntries is an int variable to store the number of entries in the file
Output: A pointer to the list of game structs
Preconditions: totalEntries is an int pointer
Postconditions: totalEntries contains the length of the gameList
**/
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
Task: Creates a new Customer struct
Input: gameInfo is a pointer to the list of all games, totalGames is the size of that list
Output: A customer struct
Preconditions: gameInfo points to a game list, totalGames is the length of gameInfo
Postconditions: customer has all fields initialized
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
Task: Print Information About This Turn
Input: hours is the number of hours that remain, money is the current money amount, and cust is the current customer
Output: None
Preconditions: hours is a double, money is a double, and cust is a customer
Postconditions: None
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
Task: Converts the number of hours passed into the current time in hours and minutes
Input: timeRemaining is the number of hours that remain, hour is the current hour, and minute is the current minute
Output: None
Preconditions: timeRemaining is a double, hour and minute are int pointers
Postconditions: hour and minute have the correct time in hours and minutes
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
Task: Prompts the user to make a choice on how to deal with a customer
Input: bins is an array containing the game bins, s is the size of the bins array, and sellGame is the name of the game the customer wants to sell
Output: the user's selection as a customerChoice enum
Preconditions: bins[] is an array of type Bin, s is an int, sellGame is a string
Postconditions: eChoice is of type customerChoice
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
Task: Checks if a game is in stock in the bins
Input: bins is an array containing the game bins, s is the size of the bins array, and n is the name of the game to check for
Output: a boolean value fo whether or not the game was found
Preconditions: bins[] is an array of type Bin, s is an int, n is a string
Postconditions: return value is a boolean
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

/**
Task: Converts the enum type gameRarity to a string
Input: the game rarity as an enum
Output: the game rarity as a string
Preconditions: rarity is of type gameRarity
Postconditions: return value is a string
**/
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
Task: Prompts the user to place the game the customer sold in one of the bins
Input: bins is an array containing the game bins, s is the size of the bins array, g is a pointer to the game to place inside, remainingHours is a pointer to the time remaining, and money is a pointer to the current money
Output: None
Preconditions: bins is an array of type Bin, size is an int, g is a game pointer, remainingHours is a double pointer, money is a double pointer
Postconditions: bins has been upated with the game, remainingHours with the new time, and money with the new profit
**/
void sellGame(Bin bins[], int size, game &g, double &remainingHours, double &money)
{
	//Declare Variables
	game* inHand = &g;
	int binSelect;

	cout << "Select the Bin in which to place " << g.name << ".\n";

	//Display the Bins
	displayBins(bins, size);

	//Prompt the User to Move Games Around
	do
	{
		cout << "Place in bin #: ";
		cin >> binSelect;
		
		//Input Validation
		if (binSelect < 1 || binSelect > size)
		{
			cout << "Error: Please Select a Valid Bin Number (Bin Numbers Start at 1)\n";
		}
		else if (bins[binSelect - 1].isFull())
		{
			cout << "Selected Bin is Full. Please try another Bin\n";
		}

	} while (binSelect < 1 || binSelect > size || bins[binSelect - 1].isFull());

	//Add the Game to the Bin
	bins[binSelect - 1].pushGame(*inHand);

	//Subtract the Purchase Amount
	money -= g.storeBuyPrice;

	//Decrease the Time Remaining
	remainingHours -= 0.25;

	//Ouput Result
	cout << g.name << " was sold to you for $" << g.storeBuyPrice << ".\n";
}

/**
Task: Prompts the User To Sort Bins to Find the Desired Game
Input: bins is an array containing the game bins, s is the size of the bins array, g is a pointer to the game to place inside, remainingHours is a pointer to the time remaining, and money is a pointer to the current money
Output: None
Preconditions: bins is an array of type Bin, size is an int, g is a game pointer, remainingHours is a double pointer, money is a double pointer
Postconditions: bins has been upated with the game, remainingHours with the new time, and money with the new profit
**/
void buyGame(Bin bins[], int size, game &g, double& remainingHours, double& money)
{
	//Declare Variables
	bool gameInHand = false;
	game* inHand = nullptr;
	game* tempGame = nullptr;
	int bin1Select;
	int bin2Select;

	cout << "Select " << g.name << " from the storage bins: \n";

	//Display the Bins
	displayBins(bins, size);

	//Prompt the User to Move Games Around
	do
	{
		do
		{
			//Display the Menu Options
			cout << "0) Place Top Game from Bin #1 in Hand\n";

			for (int i = 1; i <= size; i++)
			{
				cout << i << ") Move Game from Top of Bin #" << i << "\n";
			}
			cout << "Make a Selection: ";
			cin >> bin1Select;

			//Input Validation
			if (bin1Select < 0 || bin1Select > size)
			{
				cout << "Error: Please Select a Valid Bin Number (Bin Numbers Start at 1)\n";
			}
			else if (bin1Select != 0 && bins[bin1Select - 1].getCurrentSize() == 0)
			{
				cout << "Error: Please Select a Non-Empty Bin\n";
			}
			else if (bin1Select == 0 && bins[0].getTopGame()->name != g.name)
			{
				cout << "Error: This is not the customers desired game. Please place " << g.name << " in the first spot to proceed\n";
			}

		} while (bin1Select < 0 || bin1Select > size || (bin1Select != 0 && bins[bin1Select - 1].getCurrentSize() == 0) || (bin1Select == 0 && bins[0].getTopGame()->name != g.name));
		

		if (bin1Select == 0)
		{
			//Put the Game In Hand
			inHand = bins[0].popGame();
			gameInHand = true;
		}
		else
		{
			//Get the Game to Move
			tempGame = bins[bin1Select - 1].popGame();

			cout << "Popped Name: " << tempGame->name << endl;

			//Get the Bin To Move To
			do
			{
				//Display the Menu Options
				cout << "Select a Bin to Place the Game: ";
				cin >> bin2Select;

				//Input Validation
				if (bin2Select < 0 || bin2Select > size)
				{
					cout << "Error: Please Select a Valid Bin Number (Bin Numbers Start at 1)\n";
				}
				else if (bins[bin2Select - 1].isFull())
				{
					cout << "Error: The selected Bin is Full\n";
				}

			} while (bin2Select < 0 || bin2Select > size || bins[bin2Select - 1].isFull());

			//Add the Game to the Bin
			bins[bin2Select - 1].pushGame(*tempGame);

			//Display the Action
			cout << tempGame->name << " was moved from Bin #" << bin1Select << " to Bin #" << bin2Select << ".\n";
		}

		//Decrease the Time Remaining
		remainingHours -= 0.25;

	} while (!gameInHand);

	//Add the Payment Amount
	money += g.storeSellPrice;

	//Ouput Result
	cout << "You sold " << g.name << " for $" << g.storeSellPrice << ".\n";
}

/**
Task: Turns the Customer Away and Offers a Chance to Sort Bins
Input: bins is an array containing the game bins, size is the size of the bins array, firstName is the customer's first name, last name is the customer's last name, and remainingHours is a pointer to the time remaining
Preconditions: bins is an array of type Bin, size is an int, firstName is a string, lastName is a string, remainingHours is a double pointer
Postconditions: bins has been upated with the games, and remainingHours with the new time
**/
void noDeal(Bin bins[], int size, string firstName, string lastName, double &remainingHours)
{
	//Declare Variables
	game* tempGame = nullptr;
	int bin1Select;
	int bin2Select;

	//Display No Deal Message
	cout << "You turned " << firstName << " " << lastName << " away.\n";

	//Prompt the User to Sort the Bins
	cout << "Sort Bins Until You Are Satisfied: ";

	//Display the Bins
	displayBins(bins, size);

	//Prompt the User to Move Games Around
	do
	{
		do
		{
			//Display the Menu Options
			cout << "0) Finish Sorting\n";

			for (int i = 1; i <= size; i++)
			{
				cout << i << ") Move Game from Top of Bin #" << i << "\n";
			}
			cout << "Make a Selection: ";
			cin >> bin1Select;

			//Input Validation
			if (bin1Select < 0 || bin1Select > size)
			{
				cout << "Error: Please Select a Valid Bin Number (Bin Numbers Start at 1)\n";
			}
			else if (bin1Select != 0 && bins[bin1Select - 1].getCurrentSize() == 0)
			{
				cout << "Error: Please Select a Non-Empty Bin\n";
			}

		} while (bin1Select < 0 || bin1Select > size || (bin1Select != 0 && bins[bin1Select - 1].getCurrentSize() == 0));

		
		if (bin1Select != 0)
		{
			//Get the Game to Move
			tempGame = bins[0].popGame();

			//Get the Bin To Move To
			do
			{
				//Display the Menu Options
				cout << "Select a Bin to Place the Game\n";
				cin >> bin2Select;

				//Input Validation
				if (bin2Select < 0 || bin2Select > size)
				{
					cout << "Error: Please Select a Valid Bin Number (Bin Numbers Start at 1)\n";
				}
				else if (bins[bin2Select - 1].isFull())
				{
					cout << "Error: The selected Bin is Full\n";
				}

			} while (bin2Select < 0 || bin2Select > size || bins[bin2Select - 1].isFull());

			//Add the Game to the Bin
			bins[bin2Select - 1].pushGame(*tempGame);

			//Display the Action
			cout << tempGame->name << " was moved from Bin #" << bin1Select << " to Bin #" << bin2Select << ".\n";

			//Decrease the Time Remaining
			remainingHours -= 0.25;
		}

	} while (bin1Select != 0);

	//Decrease Time Remaining
	remainingHours -= 0.5;
}

/**
Task:
Input:
Output:
Preconditions:
Postconditions:
**/
void displayEndOfDayReport()
{

}

/**
Task: Displays a Small Report with Playthrough Stats
Input: customers is the number of customers served, money is the finishing amount of money
Output: None
Preconditions: customers is an int, money is a double
Postconditions: None
**/
void displayGameOverReport(int customers, double money)
{
	cout << "*----* WEEK OVER *----*\n";
	cout << "Total Customers Served: " << customers << "\n";
	cout << "Total Profit Earned: $" << money - STARTING_MONEY << "\n";
	cout << "\n\n";
}

/**
Task:
Input:
Output:
Preconditions:
Postconditions:
**/
void updateHighScores(int score)
{


}

/**
Task:
Input:
Output:
Preconditions:
Postconditions:
**/
void displayBins(Bin bins[], int size)
{
	//Display the Top
	for (int b = 0; b < size; b++)
	{
		cout << "*     *  ";
	}
	cout << endl;

	for (int i = bins[0].getDepth() - 1; i >= 0; i--)
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