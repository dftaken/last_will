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

protected:
   void sortCardsByScore(Cards&); // [0] = highest score
   float getCardScore(Card*);

   ActionRecord *actions;
};

#endif /* LEARNINGAI_H_ */
