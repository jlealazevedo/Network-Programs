/**********************************************************************************
Class Implementation - Rooms Base Class 
This is the implementation of the base class for the Rooms Class that will be linked 
together by the Gamestate function.
**********************************************************************************/
#include <string>
#include <iostream>
#include <stdio.h>
#include "Room.hpp"
#include "Item.hpp"
using namespace std;

//Room constructor
Room::Room(){
  visited = false;
  itemCount = 0;
  itemInventory[0] = NULL;
  features[0] = NULL;
  features[1] = NULL;
  north = NULL;
  east = NULL;
  west = NULL;
  south = NULL;

}

/*************************************************************
*Setter functions
*
**************************************************************/
void Room::setName(string roomName)
{
  name = roomName;
}

void Room::setLongDesc(string desc)
{
  longDesc = desc;
}

void Room::setShortDesc(string desc)
{
  shortDesc = desc;
}

void Room::setVisited(bool value)
{
  visited = value;
}

void Room::setNorth(Room *newRoom)
{
  north = newRoom;
}

void Room::setSouth(Room *newRoom)
{
  south = newRoom;
}

void Room::setEast(Room *newRoom)
{
  east = newRoom;
}

void Room::setWest(Room *newRoom)
{
  west = newRoom;
}

void Room::setFeature(Item *feature)
{
  if(features[0] == NULL)
  {
    features[0] = feature;
  }
  else
  {
    features[1] = feature;
  }
}

void Room::setItem(Item *item)
{
  itemInventory[itemCount] = item;
}

void Room::setItemCount(int count){
  itemCount = count;
  return;
}

//Find item in itemInventory that matches the takeName. When found
//set that slot to null and decrement itemCount for room. Used
//when player picks up an item. 
void Room::setItemToNull(string takeName){
  for(int i = 0 ; i < 8 ; i++){
    
    if(takeName == itemInventory[i]->getName()){
      itemInventory[i] = NULL;
      itemCount--;
      break;
    }
  
  }
  return;
}

//Find an empty slot in the room itemInventory. When found set that slot to
//point to the dropped item. Increment itemCount for room. Used when player
//drops and item. 
void Room::setItemToPointer(Item * dropItem){
  for(int i = 0; i < 8 ; i++){
    if(itemInventory[i] == NULL){
      itemInventory[i] = dropItem;
      itemCount++;
      break;
    }
  }
  return;
}
/*************************************************************
*Getter functions
*
**************************************************************/
string Room::getName(){
  return name;
}

string Room::getLongDesc(){
  return longDesc;
}

string Room::getShortDesc(){
  return shortDesc;
}

bool Room::getVisited(){
  return visited;
}

/*
//Unsure if this is supposed to be a string or set of objects? check with Justin on what he needs
string Room::getExits(){
  string exits = north->name + "\n" + east->name + "\n" + south->name + "\n" + west->name +"\n";
  return name;
}
*/

Room * Room::getNorth(){
  if(north != NULL){
    return north;
  }
  return NULL;
}
Room * Room::getSouth(){
  if(south != NULL){
    return south;
  }
  return NULL;
}
Room * Room::getEast(){
  if(east != NULL){
    return east;
  }
  return NULL;
}
Room * Room::getWest(){
  if(west != NULL){
    return west;
  }
  return NULL;
}

string Room::getNorthName(){
  if(north != NULL){
    return north->getName();
  }
  return "NULL";
}
string Room::getSouthName(){
  if(south != NULL){
    return south->getName();
  }
  return "NULL";
}
string Room::getEastName(){
  if(east != NULL){
    return east->getName();
  }
  return "NULL";
}
string Room::getWestName(){
  if(west != NULL){
    return west->getName();
  }
  return "NULL";
}


Item * Room::getFeature(string featureName){
//check if features are in Room
  for(int i = 0; i < 2; i++){
    if(featureName == features[i]->getName()){
      return features[i];
    }
  }
  return NULL;
}

Item * Room::getItem(string itemName){
//check inventory for the item
  for(int i = 0; i < 8; i++){
    if(itemInventory[i] != NULL){
        if(itemName == itemInventory[i]->getName()){
          return itemInventory[i];
      }
    }
  
  }
  return NULL;
}

int Room::getItemCount(){
  return itemCount;
}


//This may need to be reworked 
void Room::printItemNames(){
  
  if(itemCount != 0){
    for(int i = 0 ; i < 8 ; i++){
      if(itemInventory[i] != NULL){
        cout << "You see a " << itemInventory[i]->getName() << ". " ;
      }
    }
    cout << endl;
  }

 
}

//This may need to be reworked 
void Room::printFeatureNames(){

  for(int i = 0 ; i < 2 ; i++){
    if(itemInventory[i] != NULL){      
      cout << "You see a " << features[i]->getName() << ". ";
    }
  }
}
