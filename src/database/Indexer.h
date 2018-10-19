/*
 * Indexer.h
 *
 *  Created on: Oct 13, 2018
 *      Author: milites
 */

#ifndef INDEXER_H_
#define INDEXER_H_

#include <stddef.h>
#include <assets/Action.h>
#include <assets/cards/CardAttributes.h>

class Indexer
{
public:
   static size_t getNumActions();
   static size_t getActionNdx(
      int round,
      ActionTypeType type,
      CardIdType cardId,
      int moneyCost,
      int actionCost,
      int horseCost,
      int dogCost,
      int chefCost,
      int guestCost);
   static void convertActionNdx(
      size_t ndx,
      int &round,
      int &type,
      int &cardId,
      int &moneyCost,
      int &actionCost,
      int &horseCost,
      int &dogCost,
      int &chefCost,
      int &guestCost);

private:
   // Action Index Permutation Counts
   static int roundCount;
   static int actionTypeCount;
   static int cardIdCount;
   static int moneyCount;
   static int actionCount;
   static int companionCount;

   static int roundMod;
   static int actionTypeMod;
   static int cardIdMod;
   static int moneyMod;
   static int actionMod;
   static int companionMod;
};

#endif /* INDEXER_H_ */
