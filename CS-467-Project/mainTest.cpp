/************************************************************************************************
* Program: Adventure Time Text Based Game
* Author: Justin Azevedo
* Last Modified: 7-28-19
* Class: CS 467 Capstone Summer '19
* Description: Test main file for midway project evalution.
************************************************************************************************/
#include "GameState.hpp"
#include "Room.hpp"
#include "Item.hpp"
#include "parser.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>

using namespace std;


int main(){

	GameState * game = new GameState();
 	
 	char * path ;
 	char pathName [] = "/default";
 	char cwdName [1000];
 	memset(cwdName, '\0', sizeof(cwdName));
 	getcwd(cwdName, sizeof(cwdName));
 	strcat(cwdName, pathName);

 	path = cwdName;

 	cout << "Reading in GameState file from default folder." << endl;
 	cout << "***************************************************************************" << endl;
	game->readInGameState(path);


 	cout << "Reading in room files from default folder." << endl;
 	cout << "***************************************************************************" << endl;
	game->readInRooms(path);

	//Testing file read in
	cout << "Testing successful file read in." << endl;
	cout << "***************************************************************************" << endl;
	for(int i = 0 ; i < 17 ; i++){
		cout << game->getShipLocal(i)->getName() << endl;
		cout << "	" << "Long Desc: " << game->getShipLocal(i)->getLongDesc() << endl;
		cout << "	" << "Short Desc: " << game->getShipLocal(i)->getShortDesc() << endl;
		cout << "	" << "Visited: " << game->getShipLocal(i)->getVisited() << endl;
		cout << endl;
	}


	cout << "Testing GameState Private Variables:" << endl;
	cout << "***************************************************************************" << endl;
	cout << "Oxygen Levels: " << game->getOxygen() << endl;
	cout << "gameWon: " << game->getGameWon() << endl;
	cout << "gameQuit: " << game->getGameQuit() << endl;
	cout << "Inventory Size: " << game->getInventorySize() << endl;
	cout << "Game Intro: ";
	game->printIntro();
	cout << "Win Description: "; 
	game->printWinDesc();
	cout << "Loss Description: ";
	game->printLossDesc();

	
	game->setOxygen(1);
	cout << "Oxygen Set to 1: " << game->getOxygen() << endl;

	game->decOxygen();
	cout << "Oxygen decremented to 0: " << game->getOxygen() << endl;

	game->setInventorySize(7);
	cout << "Set inventory to 7: " << game->getInventorySize() << endl;

	game->incInventorySize();
	cout << "Increment inventory to 8: " << game->getInventorySize() << endl;

	game->decInventorySize();
	cout << "Decrement inventory to 7: " << game->getInventorySize() << endl;

	cout << "Testing Position, Current Room Name: " << game->getPosition()->getName() << endl;

	cout << "Testing Game Save functionality" << endl;

	game->_saveGame();

	//cout << "Item count in current Room: " << game->getPosition()->getItemCount() << endl;

	//cout << "Item name in current Room: " << game->getPosition()->getItem("Employee Manual")->getName() << endl;

	//cout << "Testing printing item descriptions: " ;
	//game->printItems();

	
	return 0;
}
