/**********************************************************************************
Class Definition - Item Base Class Definition
This is the definition of the base class for the items Class that will be the parent
of the features and different types of items.
**********************************************************************************/
#ifndef ARMOR_H
#define ARMOR_H
#include <string>
#include <stdio.h>
#include "Item.hpp"
using namespace std;

class Armor : public Item
{
private:
public:
	void actionFunction(string verb);
	void helperFunction();
};
#endif