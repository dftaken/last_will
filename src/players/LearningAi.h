/*
 * LearningAi.h
 *
 *  Created on: Oct 22, 2018
 *      Author: milites
 */

#ifndef LEARNINGAI_H_
#define LEARNINGAI_H_

#include <players/RandomAi.h>
#include <database/Database.h>

class LearningAi : public RandomAi
{
public:
   LearningAi(std::string);
   virtual ~LearningAi();

   virtual Cards discardCards(unsigned int numToDiscard);
   virtual Cards depreciateThreeProperties();
   virtual Cards placeTwoDogsHorses();
   virtual CardDeckTypes chooseCardDraws(int,bool=false);
//   virtual Plan::Ptr pickPlan();
//   virtual BoardSpot::Ptr pickErrand();
//   virtual void executeActions();

protected:
   void sortCardsByScore(Cards&); // [0] = highest score
   float getCardScore(Card*);
   void getBestDraws(int num,unsigned int &p, unsigned int &e, unsigned int &h, unsigned int &c);

   Record *actions;
   Record *draws;
};

#endif /* LEARNINGAI_H_ */
