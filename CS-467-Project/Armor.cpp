/**********************************************************************************
Name: Mariessa Logan
Class Implementation - Items Base Class 
This is the implementation of the base class for the items Class 
**********************************************************************************/
#include <string>
#include <iostream>
#include <stdio.h>
#include "Armor.hpp"
using namespace std;

Armor::actionFunction(string verb, Gamestate *G)
{
	//If verb is pickup, increment requirements for beating alien
	if(strcmp(verb, "pickup"))
	{
		G->requirements++;
	}
	//decrement requirements if they drop their armor
	else if(strcmp(verb, "drop"))
	{
		G->requirements--;
	}
	else if(strcmp(verb, "look"))
	{
		Armor.getDesc(verb);
	}
}