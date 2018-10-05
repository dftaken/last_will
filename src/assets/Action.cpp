/*
 * Action.cpp
 *
 *  Created on: Sep 16, 2018
 *      Author: milites
 */

#include <assets/Action.h>
#include <stdio.h>

Action::Action() :
   type(ActionType::Invalid),
   card(NULL),
   cost()
{
   // Intentionally left blank
}

Action::Action(ActionTypeType inType,Card::Ptr inCard) :
   type(inType),
   card(inCard),
   cost()
{
   // Intentionally left blank
}

Action::~Action()
{
   // Intentionally left blank
}

void Action::print()
{
   switch (type)
   {
      case ActionType::Invalid:
         printf("Invalid: ");
         break;
      case ActionType::Play:
         printf("Play: ");
         break;
      case ActionType::Upgrade:
         printf("Upgrade: ");
         break;
      case ActionType::Expense:
         printf("Expense: ");
         break;
      case ActionType::Activate:
         printf("Activate: ");
         break;
      case ActionType::Sell:
         printf("Sell: ");
         break;
      case ActionType::Discard:
         printf("Discard: ");
         break;
      default:
         printf("Unrecognized: ");
         break;
   }

   if (card != NULL)
   {
      printf("%s\n",card->strName.c_str());
   }
   else
   {
      printf("NULL\n");
   }

   for (Resources::iterator itr = cost.begin(); itr != cost.end(); ++itr)
   {
      std::string name = Resource::toString(itr->first);
      printf("   %s = %d\n",name.c_str(),itr->second);
   }
}
