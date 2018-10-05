/*
 * PlayerInterface.h
 *
 *  Created on: Sep 15, 2018
 *      Author: milites
 */

#ifndef PLAYERINTERFACE_H_
#define PLAYERINTERFACE_H_

#include <gameboards/GamePlayer.h>
#include <gameboards/GameBoardInterface.h>
#include <assets/BoardSpot.h>
#include <assets/Plan.h>
#include <assets/PlayerState.h>
#include <assets/PropertyMarket.h>
#include <vector>

class PlayerInterface : public GamePlayer
{
public:
   typedef PlayerInterface* Ptr;

   virtual ~PlayerInterface() {};

   virtual std::string getName()=0;
   virtual void setState(PlayerState::Ptr)=0;
   virtual void setBoard(GameBoardInterface::Ptr)=0;

   virtual Cards discardCards(unsigned int numToDiscard)=0;
   virtual Cards depreciateThreeProperties()=0;
   virtual Cards placeTwoDogsHorses()=0;
   virtual CardDeckTypes chooseCardDraws(int,bool=false)=0;
   virtual void takeCard(Card::Ptr)=0;

   virtual Plan::Ptr pickPlan()=0;
   virtual void takePlan(Plan::Ptr)=0;
   virtual Plan::Ptr getPlan()=0;

   virtual BoardSpot::Ptr pickErrand()=0;

   virtual PropertyAdjustments setPropertyAdjustments(Adjustments)=0;

   virtual Effects activateExpenseEffects(CardTypeType)=0;

   virtual void executeActions()=0;

protected:
   PlayerInterface() {};
};

typedef std::vector<PlayerInterface::Ptr> Players;

#endif /* PLAYERINTERFACE_H_ */
