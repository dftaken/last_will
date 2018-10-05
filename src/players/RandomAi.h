/*
 * RandomAi.h
 *
 *  Created on: Sep 15, 2018
 *      Author: milites
 */

#ifndef RANDOMAI_H_
#define RANDOMAI_H_

#include <players/PlayerInterface.h>

class RandomAi : public PlayerInterface
{
public:
   RandomAi(std::string);
   virtual ~RandomAi();

   virtual std::string getName();
   virtual void setState(PlayerState::Ptr);
   virtual void setBoard(GameBoardInterface::Ptr);

   virtual Cards discardCards(unsigned int numToDiscard);
   virtual Cards depreciateThreeProperties();
   virtual Cards placeTwoDogsHorses();
   virtual CardDeckTypes chooseCardDraws(int,bool=false);
   CardDeckTypes useAllCardDrawEffects(int &);
   virtual void takeCard(Card::Ptr);

   virtual Plan::Ptr pickPlan();
   virtual void takePlan(Plan::Ptr);
   virtual Plan::Ptr getPlan();

   virtual BoardSpot::Ptr pickErrand();

   virtual PropertyAdjustments setPropertyAdjustments(Adjustments);

   virtual Effects activateExpenseEffects(CardTypeType);
   Effects getAllExpenseEffects(CardTypeType);

   virtual void executeActions();

   void print();
protected:
   std::string myName;
   PlayerState::Ptr myState;
   GameBoardInterface::Ptr gameboard;
};

#endif /* RANDOMAI_H_ */
