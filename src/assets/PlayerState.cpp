/*
 * PlayerState.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#include <assets/PlayerState.h>
#include <Logging.h>

using namespace std;

PlayerState::PlayerState(std::string inName) :
   name(inName),
   resources(),
   board(inName),
   hand(inName),
   plan(NULL),
   finalPropertyCount(0),
   finalPlanPriority(0)
{
   // Intentionally left blank
}

PlayerState::~PlayerState()
{
   // Intentionally left blank
}

string PlayerState::getName()
{
   return name;
}

void PlayerState::releasePlan()
{
   if (plan != NULL)
   {
      finalPlanPriority = plan->priority;
      plan->location = NULL;
      plan = NULL;
   }
}

void PlayerState::takePlan(Plan::Ptr ptr)
{
   if (ptr != NULL && ptr->location == NULL && plan == NULL)
   {
      plan = ptr;
      plan->location = this;
   }
}

unsigned int PlayerState::getMaxHandSize()
{
   unsigned int maxHandSize = 2;
   CardSpots spots = board.getOccupiedSpots();
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      Card::Ptr card = (*itr)->getCard();
      if (card != NULL && card->effect == Effect::Plus2MaxHandSize)
      {
         maxHandSize += 2;
      }
   }
   return maxHandSize;
}

unsigned int PlayerState::getHandSize()
{
   return hand.getSize();
}

bool PlayerState::canSpendMoney(int moneyAmount)
{
   if (resources[Resource::Money] > moneyAmount)
   {
      return true;
   }
   else
   {
      bool haveUnsoldProperty = false;
      Cards cards = board.getCards();
      for (size_t i = 0; i < cards.size(); ++i)
      {
         if (cards[i]->group == CardGroup::Property)
         {
            haveUnsoldProperty = true;
            break;
         }
      }
      return !haveUnsoldProperty;
   }
}

void PlayerState::reset()
{
   resources.clear();
   board.reset();
   hand.reset();
   releasePlan();
   resources[Resource::BoardSpot] = board.getNumAvailSpots();
   finalPropertyCount = 0;
   finalPlanPriority = 0;
}

void PlayerState::print()
{
   printf("%s State:\n",name.c_str());
   board.print();
   hand.print();
   resources.print();
}
