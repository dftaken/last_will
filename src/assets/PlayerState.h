/*
 * PlayerState.h
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#ifndef PLAYERSTATE_H_
#define PLAYERSTATE_H_

#include <assets/Plan.h>
#include <assets/PlayerBoard.h>
#include <assets/PlayerHand.h>

#include <string>

class PlayerState : public Plan::Location
{
public:
   typedef PlayerState* Ptr;

   PlayerState(std::string);
   ~PlayerState();

   // Plan::Location Functions
   std::string getName();
   void releasePlan();
   void takePlan(Plan::Ptr);

   unsigned int getMaxHandSize();
   unsigned int getHandSize();

   bool canSpendMoney(int);

   int getNumWildcards();
   int getNumDogs();
   int getNumActions();

   void reset();

   void print();

   // Data
   std::string name;
   Resources resources;
   PlayerBoard board;
   PlayerHand hand;
   Plan::Ptr plan;

   // Only used for final determination of winner
   int finalPropertyCount;
   int finalPlanPriority;
};

#endif /* PLAYERSTATE_H_ */
