/*
 * GameMaster.h
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#ifndef GAMEMASTER_H_
#define GAMEMASTER_H_

// Game Boards
#include <gameboards/GameBoard.h>

// Players
#include <players/PlayerInterface.h>

class GameMaster
{
public:
   GameMaster();
   ~GameMaster();

   void run(int, char**);

protected:
   void processCmdLine(int,char**);

   GameBoard::Ptr gameboard;
   Players players;

   void planningPhase();
   void errandsPhase();
   void actionsPhase();

   void givePlayerCard(PlayerInterface::Ptr,Card::Ptr);
   void grantPlayerResources(PlayerInterface::Ptr,Plan::Ptr);
   bool validateCardChoices(PlayerInterface::Ptr,CardDeckTypes);
   void removeAChoice(CardDeckTypes &, CardDeckType);
   void processErrand(PlayerInterface::Ptr,BoardSpot::Ptr);
   bool validatePropertyAdjustments(PropertyAdjustments);
   bool validatePlayerEffects(PlayerInterface::Ptr,Effects);
};

#endif /* GAMEMASTER_H_ */
