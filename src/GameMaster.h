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

#include <pthread.h>

class GameMaster;

struct ThreadData
{
   GameMaster *gm;
   long unsigned int numGames;
};

class GameMaster
{
public:
   GameMaster();
   ~GameMaster();

   void run(int, char**);

   void executeGame();

protected:
   void processCmdLine(int,char**);
   void kickOffThread();

   GameBoard::Ptr gameboard;
   Players players;
   pthread_t tid;
   ThreadData data;

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
