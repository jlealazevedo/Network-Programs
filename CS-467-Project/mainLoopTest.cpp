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


int main() {

	GameState * game = new GameState();



 	char * path ;
 	char pathName [] = "/default";
 	char cwdName [1000];
 	memset(cwdName, '\0', sizeof(cwdName));
 	getcwd(cwdName, sizeof(cwdName));
 	strcat(cwdName, pathName);
 	path = cwdName;
	
	game->readInGameState(path);
	game->readInRooms(path);

	game->printIntro();

	while(game->getGameQuit() == false){
	

		string input; 
		vector<string> parsedLine;
		
		game->printCurRoomDesc();

		//PRINT STATEMENT FOR DEBUGGING
		cout << "Current room Name: " << game->getPosition()->getName() << endl;
		cout << "O2 Level: " << game->getOxygen() << endl;
		
		game->printExits();

		cout << "> ";

		getline(cin, input);

		cout << "\n";

		parsedLine = parse(input, ' ');

		game->enactVerb(parsedLine);

		input.clear();

	}


	return 0;
}

