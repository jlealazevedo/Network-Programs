/***********************************************************************
Program: Adventure Time CLI Game
Author: Hamilton Skrabanek
Date: 7/17/19
Description: Parsing engine that validates user input. It will return a 
	vector of strings containing either a single valid verb, or 
	an actionable verb and corresponding noun
***********************************************************************/ 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "parser.hpp"
using namespace std;

//splits a user input string by spaces and returns a vector of strings
//ultimately the vector will only contain either 1 or 2 strings
vector<string> parse(const string& inputStr, char delim)
{
	string lowerCaseStr;
	vector<string> commands;
	vector<string> spaceGame = {"space!"};

	//convert input to lower case
	for (size_t i = 0; i < inputStr.length(); i++) {
		lowerCaseStr.push_back(tolower(inputStr[i]));
	}

	istringstream iss(lowerCaseStr);
	string token;

	//split input by delimiter and push to a vector
	while (getline(iss, token, delim)) {
		commands.push_back(token);
	}

	//check for double verbs up front
	if (commands.size() == 2) {
		commands = concatDoubleVerbs(commands);
		commands = concatDoubleMovements(commands);
	}

	if (commands.size() == 1) {
		commands = validateSingleEntry(commands);
		return commands;
	}

	if (commands.size() == 2) {
		commands = validateDoubleEntry(commands);
		return commands;
	}

	if (commands.size() == 3) {
		commands = validateTripleEntry(commands);
		return commands;
	}

	if (commands.size() >= 4) {
		commands = validateManyEntry(commands);
		return commands;
	}

	return spaceGame;
}


/**************************************************************************
*These 3 functions test that the input is recognized in the game's lexicon
***************************************************************************/
bool validateVerb(vector<string> commands)
{
	bool valid = false;

	for(size_t i = 0; i < verbs.size(); i++){
		if(commands[0] == verbs[i]){
			valid = true;
		}
	}

	return valid;
}


bool validateMovement(vector<string> commands, int indexToCheck) {
	bool valid = false;

	for (size_t i = 0; i < exits.size(); i++) {
		if (commands[indexToCheck] == exits[i]) {
			valid = true;
		}
	}

	return valid;
}


bool validateNoun(vector<string> commands)
{
	bool found = false;

	for (size_t i = 1; i < commands.size(); i++) {
		for (size_t j = 0; j < nouns.size(); j++) {
			if (commands[i] == nouns[j]) {
				found = true;
			}
		}
	}

	return found;
}


vector<string> validateSingleEntry(vector<string> &commands) {
	bool validVerb = false;
	bool validMovement = false;
	bool foundSoloVerb = false;

	validVerb = validateVerb(commands);
	
	//if single entry is a valid verb
	if (validVerb) {

		//check that it is not a movement verb without a destination
		for (size_t i = 0; i < movementVerbs.size(); i++) {
			if (commands[0] == movementVerbs[i]) {
				commands.clear();
				commands.push_back("Please enter an exit or direction...");
				return commands;
			}
		}

		//check if entry can act as a single verb
		for (size_t i = 0; i < soloVerbs.size(); i++) {
			if (commands[0] == soloVerbs[i]) {
				foundSoloVerb = true;
			}
		}

		if (!foundSoloVerb) {
			commands.clear();
			commands.push_back("Please enter a verb/noun combination...");
			return commands;
		}
		else {
			return commands;
		}
	}

	//if single entry and not a verb, then check for an exit
	if (!validVerb) {
		validMovement = validateMovement(commands, 0);
	}

	if (validMovement) {
		commands.insert(commands.begin(), "go");
		return commands;
	}

	//if verb is invalid and movement valid - return as valid input
	if (!validVerb && validMovement) {
		return commands;
	}

	commands.clear();
	commands.push_back("Invalid entry...");
	return commands;
}


vector<string> validateDoubleEntry(vector<string> &commands) {
	bool validVerb = false;
	bool validMovement = false;
	bool validNoun = false;

	validVerb = validateVerb(commands);

	validMovement = validateMovement(commands, 1);

	validNoun = validateNoun(commands);

	if (validVerb && validMovement) {
		commands = swapVerbs(commands);
		return commands;
		/*
		if (commands[0] == "go") {
			return commands;
		}

		if (commands[0] == "move") {
			commands[0] = "go";
			return commands;
		}*/
	}

	if (validVerb && validNoun) {
		commands = swapVerbs(commands);

		if (commands[0] == "look at") {
			return commands;
		}

		commands = validateCombo(commands);
		return commands;
	}

	commands.clear();
	commands.push_back("Invalid entry...");
	return commands;
}


vector<string> validateTripleEntry(vector<string> &commands) {
	//check for double verbs
	commands = concatDoubleVerbs(commands);
	
	//check for double nouns
	commands = concatDoubleNouns(commands);
	commands = concatDoubleMovements(commands);

	if (commands.size() == 2) {
		commands = validateDoubleEntry(commands);
		return commands;
	}

	if (commands.size() == 3) {
		commands = stripTrashWords(commands);
		commands = validateDoubleEntry(commands);
		return commands;
	}

	commands.clear();
	commands.push_back("Invalid entry...");
	return commands;
}


vector<string> validateManyEntry(vector<string> &commands) {
	//check for double verbs
	commands = concatDoubleVerbs(commands);

	//check for double nouns
	commands = concatDoubleNouns(commands);
	commands = concatDoubleMovements(commands);

	if (commands.size() == 2) {
		commands = validateDoubleEntry(commands);
		return commands;
	}

	if (commands.size() >= 3) {
		commands = stripTrashWords(commands);
		commands = validateDoubleEntry(commands);
		return commands;
	}

	commands.clear();
	commands.push_back("Invalid entry...");
	return commands;
}

/***************************************************************************************
*These functions recognize and concatenate dictionary terms with a space into one entry
****************************************************************************************/
vector<string> concatDoubleMovements(vector <string> &commands) {
	int index = getDoubleExitIndex(commands);

	if (index == -1) {
		return commands;
	}

	if (commands[index] == "quarters" && commands[index - 1] == "captains") {
		commands[index - 1] = "captains quarters";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "room" && commands[index - 1] == "strategy") {
		commands[index - 1] = "strategy room";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "room" && commands[index - 1] == "transporter") {
		commands[index - 1] = "transporter room";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "room" && commands[index - 1] == "command") {
		commands[index - 1] = "command room";
		commands.erase(commands.begin() + index);
		return commands;
	}


	if (commands[index] == "room" && commands[index - 1] == "break") {
		commands[index - 1] = "break room";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "closet" && commands[index - 1] == "supply") {
		commands[index - 1] = "supply closet";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "pod" && commands[index - 1] == "escape") {
		commands[index - 1] = "escape pod";
		commands.erase(commands.begin() + index);
		return commands;
	}

	return commands;
}

int getDoubleExitIndex(vector<string> commands) {
	int foundIdx = -1;

	vector<string> doubleExits = {
		"quarters", "room", "pod", "closet"
	};

	for (size_t i = 0; i < commands.size(); i++) {
		for (size_t j = 0; j < doubleExits.size(); j++) {
			if (commands[i] == doubleExits[j]) {
				foundIdx = i;
			}
		}
	}

	return foundIdx;
}


vector<string> concatDoubleVerbs(vector<string> &commands) {
	if (commands[0] == "look" && commands[1] == "at") {
		commands[0] = "look at";
		commands.erase(commands.begin() + 1);
	}

	if (commands[0] == "turn" && commands[1] == "on") {
		commands[0] = "turn on";
		commands.erase(commands.begin() + 1);
	}

	if (commands[0] == "turn" && commands[1] == "off") {
		commands[0] = "turn off";
		commands.erase(commands.begin() + 1);
	}

	if (commands[0] == "pick" && commands[1] == "up") {
		commands[0] = "pick up";
		commands.erase(commands.begin() + 1);
	}

	return commands;
}


int getDoubleNounIndex(vector <string> commands) {
	int foundIdx = -1;

	vector<string> doubleNouns = {
		"manual", "badge", "key", "goggles", "maker",
		"meter", "pipe", "supplies", "monitor","suit"
	};

	for(size_t i = 0; i < commands.size(); i++) {
		for (size_t j = 0; j < doubleNouns.size(); j++) {
			if (commands[i] == doubleNouns[j]) {
				foundIdx = i;
			}
		}
	}

	return foundIdx;
}


vector<string> concatDoubleNouns(vector<string> &commands){

	int index = getDoubleNounIndex(commands);

	if (index == -1) {
		return commands;
	}

	if(commands[index] == "manual" && commands[index - 1] == "employee"){
		commands[index - 1] = "employee manual";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "badge" && commands[index - 1] == "security") {
		commands[index - 1] = "security badge";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "key" && commands[index - 1] == "desk") {
		commands[index - 1] = "desk key";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "goggles" && commands[index - 1] == "thermal") {
		commands[index - 1] = "thermal goggles";
		commands.erase(commands.begin() + index);
		return commands;
	}
	
	if (commands[index] == "maker" && commands[index - 1] == "coffee") {
		commands[index - 1] = "coffee maker";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "meter" && commands[index - 1] == "oxygen") {
		commands[index - 1] = "oxygen meter";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "pipe" && commands[index - 1] == "steam") {
		commands[index - 1] = "steam pipe";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "supplies" && commands[index - 1] == "surgical") {
		commands[index - 1] = "surgical supplies";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "monitor" && commands[index - 1] == "heart") {
		commands[index - 1] = "heart monitor";
		commands.erase(commands.begin() + index);
		return commands;
	}

	if (commands[index] == "suit" && commands[index - 1] == "mech") {
		commands[index - 1] = "mech suit";
		commands.erase(commands.begin() + index);
		return commands;
	}

	return commands;
	
}


//removes unnecessary articles and prepositions from the input
vector<string> stripTrashWords(vector<string> &commands)
{

	for (size_t i = 0; i < commands.size(); i++) {
		for (size_t j = 0; j < removables.size(); j++) {
			if (commands[i] == removables[j]) {
				commands.erase(commands.begin() + i);
				i--;
			}
		}
	}

	return commands;
}


//allows the user to enter adjectives for game verbs
vector<string> swapVerbs(vector<string> &commands) {
	//go
	if (commands[0] == "move") {
		commands[0] = "go";
	}
	//take
	if (commands[0] == "grab" || commands[0] == "pick up") {
		commands[0] = "take";
	}
	//drop
	if (commands[0] == "leave") {
		commands[0] = "drop";
	}
	//read
	if (commands[0] == "read") {
		commands[0] = "look at";
	}
	//close
	if (commands[0] == "shut") {
		commands[0] = "close";
	}
	//shoot
	if (commands[0] == "fire") {
		commands[0] = "shoot";
	}
	return commands;
}


vector<string> validateCombo(vector<string> &commands) {
	typedef multimap<string, string> dicto;
	bool valid = false;

	dicto dict = {
	//take items
		{"take", "phaser"},
		{"take", "armor"},
		{"take", "radio"},
		{"take", "employee manual"},
		{"take", "security badge"},
		{"take", "desk key"},
		{"take", "charger"},
		{"take", "thermal goggles"},
	//drop items
		{"drop", "phaser"},
		{"drop", "armor"},
		{"drop", "radio"},
		{"drop", "employee manual"},
		{"drop", "security badge"},
		{"drop", "desk key"},
		{"drop", "charger"},
		{"drop", "thermal goggles"},
	//open
		{"open", "employee manual"},
		{"open", "dresser"},
		{"open", "dryer"},
		{"open", "binder"},
		{"open", "safe"},
		{"open", "desk"},
	//close
		{"close", "employee manual"},
		{"close", "dresser" },
		{"close", "dryer" },
		{"close", "binder" },
		{"close", "safe" },
		{"close", "desk" },
	//push
		{"push", "dresser"},
		{"push", "chair"},
		{"push", "button"},
		{"push", "desk"},
	//turn on 
		{"turn on", "television"},
		{"turn on", "mech suit"},
		{"turn on", "coffee maker"},
	//turn off
		{ "turn off", "television" },
		{ "turn off", "mech suit" },
		{ "turn off", "coffee maker" },
	//drink
		{"drink", "coffee"},
		{"drink", "beaker"},
		{"drink", "alcohol"},
	//scan
		{"scan", "security badge"},
		{"scan", "desk key"},
	//shoot
		{"shoot", "alien"},
	//shake
		{"shake", "lopez"},
		{"shake", "kelvin"},
		{"shake", "melvin"},
	//listen
		{"listen", "cd"},
		{"listen", "radio"}
	};

	dicto::iterator pos;

	string key = commands[0];
	string value = commands[1];
	for (pos = dict.lower_bound(key); pos != dict.upper_bound(key); ++pos) {
		if (pos->second == value) {
			valid = true;
		}
	}

	if (valid) {
		return commands;
	}
	else {
		commands.clear();
		commands.push_back("You can't do that...");
		return commands;
	}
}
