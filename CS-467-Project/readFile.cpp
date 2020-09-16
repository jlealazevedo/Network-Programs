/************************************************************************************************
* Program: Adventure Time Text Based Game
* Author: Justin Azevedo
* Last Modified: 7-28-19
* Class: CS 467 Capstone Summer '19
* Description: Unit tests for parser 
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

using namespace std;

void readRoomFile(){

	string roomName; 
	string shortDesc;
	string longDesc; 


}

void readItemFile(){
	string name;
	string desc1;
	string pickupString;
	bool pickup;
	string desc2;
	string secondVerb;

	fstream inputFile;
	inputFile.open("sampleItem.txt");

	getline(inputFile, name); 
	getline(inputFile, desc1);
	getline(inputFile, pickupString);
	getline(inputFile, desc2);
	getline(inputFile, secondVerb);

	cout << "Testing item file read:" << endl;
	cout << "Name: " << name << endl;
	cout << "desc1: " << desc1 << endl;
	cout << "pickup: " << pickupString << endl;
	cout << "desc2: " << desc2 << endl;
	cout << "secondVerb: " << secondVerb << endl;

	if(pickupString == "true"){
		pickup = true;
	}
	else{
		pickup = false;
	}

	Item * testItem = new Item(name, desc1, pickup, desc2, secondVerb);

	cout << endl;
	cout << "Testing item constructor: " << endl;
	cout << "Name: " << testItem->getName() << endl;
	cout << "Desc1: " << testItem->getDesc1() << endl;
	cout << "Pickup: " << testItem->getPickup() << endl;
	cout << "Desc2: " << testItem->getDesc2() << endl;
	cout << "SecondVerb: " << testItem->getSecondVerb() << endl;



	inputFile.close();
	free(testItem);
}




int main(){

	readItemFile();

	return 0;
}