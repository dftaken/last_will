/*
 * Action.h
 *
 *  Created on: Sep 16, 2018
 *      Author: milites
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <assets/cards/Card.h>
#include <assets/cards/CardAttributes.h>

struct ActionType
{
   enum Type {
      Invalid=-1,
      Play, // Play an event or put a card on board
      Upgrade, // Upgrade on a card on board
      Expense,
      Activate, // Only applies to cards on player board
      Sell,
      Discard
   };
};
typedef ActionType::Type ActionTypeType;

class Action
{
public:
   Action();
   Action(ActionTypeType,Card::Ptr=NULL);
   ~Action();

   void print();

   ActionTypeType type;
   Card::Ptr card;
   Resources cost;
};

typedef std::vector<Action> Actions;

#endif /* ACTION_H_ */
