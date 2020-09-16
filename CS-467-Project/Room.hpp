/**********************************************************************************
Author: Mariessa Logan
Class Definition - Rooms Base Class Definition
This is the definition of thebase class for the Rooms Class that will be linked 
together by the Gamestate function.
**********************************************************************************/
#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <stdio.h>
#include "Item.hpp"
using namespace std;
//include gamestate without circular dependency
class GameState;

//class declaration
class Room
{
  private:
    string name;
    string shortDesc;
    string longDesc;
    bool visited;
    int itemCount;
    Item * itemInventory[8];
    Item * features[2];
    Room * north;
    Room * east;
    Room * south;
    Room * west;
   
   public:
    //functions
    Room();
    //set functions
    void setName(string roomName);
    void setLongDesc(string desc);
    void setShortDesc(string desc);
    void setVisited(bool visit);
    void setNorth(Room *newRoom);
    void setSouth(Room *newRoom);
    void setEast(Room *newRoom);
    void setWest(Room *newRoom);
    void setFeature(Item * feature);
    void setItem(Item * item);
    void setItemCount(int count);
    void setItemToNull(string takeName);
    void setItemToPointer(Item * dropItem); 

    //get functions
    string getName();
    string getLongDesc();
    string getShortDesc();
    bool getVisited();
    string getExits();
    Room * getNorth();
    Room * getSouth();
    Room * getEast();
    Room * getWest();
    string getNorthName();
    string getSouthName();
    string getEastName();
    string getWestName();
    Item * getFeature(string featureName);
    Item * getItem(string itemName);
    int getItemCount();

    //other functions
    void printItemNames();
    void printFeatureNames();
    virtual void actionFunction(){};
};
#endif
