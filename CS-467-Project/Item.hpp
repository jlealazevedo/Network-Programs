/**********************************************************************************
Class Definition - Item Base Class Definition
This is the definition of the base class for the Items Class that will be the parent
of the features and different types of Items.
**********************************************************************************/
#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <stdio.h>
#include <cstring>
using namespace std;
class Item
{
  private:
    string name;
    string desc1;
    string desc2;
    bool pickup;
    string secondVerb;
    bool charged;//for phaser, but should we do locked
    string locationName;//for location of item
   
   public:
    //functions
    Item();
    
    //add setters and getters
    string getName();
    string getDesc1();
    string getDesc2();
    string getSecondVerb();
    void getDesc(string verb);
    bool getPickup();

    //virtual function for actions on unique items
    virtual void actionFunction(string verb);
};
#endif
