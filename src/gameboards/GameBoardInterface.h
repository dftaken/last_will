/*
 * GameBoardInterface.h
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#ifndef GAMEBOARDINTERFACE_H_
#define GAMEBOARDINTERFACE_H_

#include <assets/Action.h>
#include <assets/BoardSpot.h>
#include <assets/Plan.h>
#include <assets/PlayerState.h>
#include <gameboards/GamePlayer.h>

#include <string>

class GameBoardInterface
{
public:
   typedef GameBoardInterface* Ptr;

   virtual ~GameBoardInterface() {};

   virtual Plans getAvailablePlans()=0;
   virtual Plans getAllPlans()=0;

   virtual BoardSpots getAvailableErrands(GamePlayer::Ptr)=0;
   virtual BoardSpots getAllErrands(GamePlayer::Ptr)=0;

   virtual Actions getAvailableActions(GamePlayer::Ptr)=0;
   virtual bool processAction(GamePlayer::Ptr,Action)=0;

protected:
   GameBoardInterface() {};
};

#endif /* GAMEBOARDINTERFACE_H_ */
