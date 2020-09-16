/**********************************************************************************
Name: Mariessa Logan
Class Definition - Holodeck Class Definition
This is the definition of the holodeck class which is a child of  the rooms Class 
that will be linked together by the Gamestate function.
**********************************************************************************/
#ifndef HOLODECK_H
#define HOLODECK_H
#include <string>
#include <stdio.h>
#include "Room.hpp"
#include "GameState.hpp"

using namespace std;

class Holodeck
{
  private:
    bool pushed;
   
   public:
    //functions
    void actionFunction(GameState *G);
    
    
};
#endif
