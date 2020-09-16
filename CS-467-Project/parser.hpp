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
using namespace std;

//global arrays for referencing lexicon of the game
static const vector<string> verbs = 
		{"look", "look at","go", "move", "take", "pick up", "grab", "help", "inventory",
		 "drop", "leave", "save", "load", "quit", "open", "close", 
		"shut", "push", "turn on", "turn off", "drink", "shake", "read",
		"scan", "shoot", "fire", "listen"};

static const vector<string> nouns = 
		{"employee manual", "phaser", "armor", "radio", "security badge", "desk key",
		"thermal goggles", "charger", "dresser", "photo", "lopez", "television", "bar", "cd",
		"scanner", "dryer", "mirror", "menu", "coffee maker", "coffee", "button",
		"oxygen meter", "chair", "binder", "starbuck", "mal", "safe", "mech suit", "blowtorch",
		"uniform", "beaker", "desk", "steam pipe", "kirk", "transporter", "alien", 
		"screen", "melvin", "kelvin", "surgical supplies", "heart monitor", "port", "alcohol", "beaker",
		};

static const vector<string> exits =
		{"north", "south", "east", "west", "bunk", "break room", "captains quarters",
		"bathroom", "cafeteria", "holodeck", "strategy room", "armory", "quartermaster",
		"workshop", "lab", "corridor", "transporter room", "command room", "supply closet",
		"medbay", "escape pod"};

static const vector<string> removables =
		{ "under", "on", "off", "all", "the", "above", "below",
			"to", "a", "at", "toward", "towards", "those", "these",
			"over", "across"
		};

static const vector<string> soloVerbs =
		{ "look", "help", "inventory", "help", "save", "load", "quit" };

static const vector<string> movementVerbs = { "go", "move" };

bool validateVerb(vector<string> commands);
bool validateMovement(vector<string> commands, int index);
bool validateNoun(vector<string> commands);

vector<string> validateSingleEntry(vector<string> &commands);
vector<string> validateDoubleEntry(vector<string> &commands);
vector<string> validateTripleEntry(vector<string> &commands);
vector<string> validateManyEntry(vector<string> &commands);
vector<string> validateCombo(vector<string> &commands);
vector<string> swapVerbs(vector<string> &commands);

vector<string> concatDoubleVerbs(vector<string> &commands);
int getDoubleNounIndex(vector <string> commands);
vector<string> concatDoubleNouns(vector<string> &commands);
vector<string> concatDoubleMovements(vector <string> &commands);
int getDoubleExitIndex(vector<string> commands);
vector<string> stripTrashWords(vector<string> &commands);

vector<string> parse(const string& input, char delim);