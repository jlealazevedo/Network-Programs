/*****************************************************************************
* Program: Adventure Time Text Based Game
* Author: Justin Azevedo
* Date: 7/15/19
* Class: CS 467 Capstone Summer '19
* Description: Header file for the GameState class. 
******************************************************************************/
#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "Room.hpp"									
#include "Item.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>

using namespace std;

class GameState{

	private: 
		Room * ship [17];
		Room * position;
		int oxygen; 
		bool gameWon;
		bool gameLost;
		bool gameQuit;
		string verbList [20]; 
		string intro;
		string winDesc;								//Functions for these not written, check with group about usage.
		string lossDesc;							//" "
		Item * inventory [8];
		int inventorySize;

	public: 
		//Constructor and Destructor
		GameState(); 										//Default constructor
		//_checkForSaveFile();								//Look for save directory, if found look for files. 
		~GameState();										//Deconstructor
		void linkRooms();									//Connect all room exits.
		void readInGameState(char * path);
		void readInRooms(char * path);
		void saveGameState();
		void saveRooms();
		
		//Getter functions
		Room * getShipLocal(int roomNum);				//Return a pointer to a ship location specified by the roomNum;
		Room * getPosition(); 							//Return pointer to current room
		int getOxygen();								//Return Oxygen levels
		bool getGameWon();								//Return gameWon bool, default is false, set to tre if player wins game.
		bool getGameLost();								//Return gameLost bool, default is false.
		bool getGameQuit();								//Return gameQuit bool, default is false, set to true if player quits game
		int getInventorySize();							//Return number of items in inventory
		
		//Print functions
		void printIntro();								//Print game intro for new game
		void printExits();								//Print names of adjacent rooms for exits. 
		void printWinDesc();							//Print game won message
		void printLossDesc();							//Print game loss message
		void printCurRoomDesc();						//Prints long or short desc for room depending on room.visted bool. Sets visited to true after printing the long desc.
		void printItems();								//Prints all the item descriptions for a room
		void printFeatures();							//Prints all the feature descriptions for a room. 

		//Setter / Modifier functions
		void setPosition(Room * newRoom);				//Set position to new room
		void setOxygen(int oxygenLevel);				//Set oxygen level
		void decOxygen();								//Decrement oxygen. Call only after room change. 
		void setGameWon(bool state);					//Set gameWon bool	
		void setGameLost(bool state);					//Set gameLost bool
		void setGameQuit(bool state);					//Set gameQuit bool
		void setInventorySize(int numItems);			//Set inventory size to specific value.
		void incInventorySize();						//Increase inventory size by one.
		void decInventorySize();						//Decrease inventory size by one.
		

		//Pasers functions
		
		//Helper functions used inside enactVerb	
		
		Item * _checkForFeature(string noun);				//Check if noun is a feature in the current room. Return a pointer to feature if found, else returns null
		Item * _checkForItem(string noun);					//Check if noun is an item in the current room. Return a pointer to the item if present, else returns null. 
		Item * _checkInventory(string noun);				//Check if noun matches name of an item in player inventory. Return a pointer to the item if present, else returns null. 
		bool _checkCurrentRoom(string noun);				//Check if noun is name of the current room.
		void _look();									//Prints long room description
		void _lookAt(string noun);						//Prints description of item / feature
		void _moveRooms(string noun);					//Update position pointer call decOxygen()
		void _takeItem(string noun);					//Add item to inventory
		void _dropItem(string noun);					//Drop item from inventory into room. 
		void _help();									//Prints contents of verb list
		void _printInventory();							//Prints names of items in inventory
		void _saveGame();
		void _quitGame();
		void _itemAction(string verb, string noun, GameState game);	//Calls unique function of item / feature
		void enactVerb(vector<string> parsedInput);		//Main switch function for receiving input from parser
		
	};

	#endif