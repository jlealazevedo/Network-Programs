/**********************************************************************************
Name: Mariessa Logan
Class Implementation - Holodeck Class 
This is the implementation of the holodeck that will affect the oxygen level
**********************************************************************************/
#include <string>
#include <iostream>
#include <stdio.h>
#include "Holodeck.hpp"
using namespace std;

  void Holodeck::actionFunction(GameState *G)
  {
    if(pushed == false)
    {
      //find out current oxygen on gamestate and add 5
      int oxygen = G->getOxygen();
      oxygen += 5;
      G->setOxygen(oxygen);
      //set button pushed to true so it knows to not increment by 5 again.
      pushed = true;
    }
    else
    {
      //button has been pushed
      cout << "The holodeck is already turned off.\n";
    }
  }

 
