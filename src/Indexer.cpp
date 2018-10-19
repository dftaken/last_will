/*
 * Indexer.cpp
 *
 *  Created on: Oct 13, 2018
 *      Author: milites
 */

#include <Indexer.h>
#include <stdexcept>

/* Round Permutations
 * 1 - 7
 */
int Indexer::roundCount = 7;

/* Action Type Permutations
 * 0 - Play
 * 1 - Upgrade
 * 2 - Expense
 * 3 - Activate
 * 4 - Sell
 * 5 - Discard
 */
int Indexer::actionTypeCount = 6;

/* Card Id Permutations
 * Card Ids Farm_1 - Steward_S2
 * # = 111
 */
int Indexer::cardIdCount = 111;

/* Money Cost Permutations
 * -8 - 30
 * For sell actions, money cost is multiplied by -1 since the
 * rest of the game uses negative cost to translate to money
 * going back to the player.
 */
int Indexer::moneyCount = 39;

/* Action Count Permutations
 * 0 - 7
 */
int Indexer::actionCount = 8;

/* Companion Cost Permutations
 * H (horse), D (dog), G (guest), C (chef)
 * None, H, D, G, C, GDC, GD, GC, DC, GG, CC
 * ONLY FOR 15: HH, DH, DD, DDD
 */
int Indexer::companionCount = 11;
//int Indexer::companionCount = 15;

/*****************************************************************************/

size_t Indexer::getNumActions()
{
   static size_t numActions =
      roundCount * actionTypeCount * cardIdCount *
      moneyCount * actionCount * companionCount;
   return numActions;
}

size_t Indexer::getActionNdx(
   int round,
   ActionTypeType type,
   CardIdType cardId,
   int moneyCost,
   int actionCost,
   int h, // horse cost
   int d, // dog cost
   int c, // chef cost
   int g) // guest cost
{
   // Round
   if (round < 1 || round > 7)
      throw std::runtime_error("Round outside of expected bounds.");
   unsigned int roundNdx = round - 1;

   // Action Type
   if (type < ActionType::Play || type > ActionType::Discard)
      throw std::runtime_error("ActionType outside of expected bounds.");
   unsigned int typeNdx = type - ActionType::Play;

   // Card Id
   if (cardId < CardId::Farm_1 || cardId > CardId::Steward_S2)
      throw std::runtime_error("CardId outside of expected bounds.");
   unsigned int cardNdx = cardId - CardId::Farm_1;

   // Money Cost
   int mCost = type == ActionType::Sell ? moneyCost * -1 : moneyCost;
   if (mCost < -8 || mCost > 30)
      throw std::runtime_error("Money cost outside of expected bounds.");
   unsigned int moneyNdx = mCost + 8;

   // Action Cost
   if (actionCost < 0 || actionCost > 7)
      throw std::runtime_error("Action cost outside of expected bounds.");
   unsigned int actionNdx = actionCost;

   // Companion Cost
   unsigned int companionNdx = 0;
   if (h == 0 && d == 0 && c == 0 && g == 0)
      companionNdx = 0;
   else if (h == 1 && d == 0 && c == 0 && g == 0)
      companionNdx = 1;
   else if (h == 0 && d == 1 && c == 0 && g == 0)
      companionNdx = 2;
   else if (h == 0 && d == 0 && c == 1 && g == 0)
      companionNdx = 3;
   else if (h == 0 && d == 0 && c == 0 && g == 1)
      companionNdx = 4;
   else if (h == 0 && d == 1 && c == 1 && g == 1)
      companionNdx = 5;
   else if (h == 0 && d == 1 && c == 0 && g == 1)
      companionNdx = 6;
   else if (h == 0 && d == 0 && c == 1 && g == 1)
      companionNdx = 7;
   else if (h == 0 && d == 1 && c == 1 && g == 0)
      companionNdx = 8;
   else if (h == 0 && d == 0 && c == 0 && g == 2)
      companionNdx = 9;
   else if (h == 0 && d == 0 && c == 2 && g == 0)
      companionNdx = 10;
//   else if (h == 2 && d == 0 && c == 0 && g == 0)
//      companionNdx = 11;
//   else if (h == 1 && d == 1 && c == 0 && g == 0)
//      companionNdx = 12;
//   else if (h == 0 && d == 2 && c == 0 && g == 0)
//      companionNdx = 13;
//   else if (h == 0 && d == 3 && c == 0 && g == 0)
//      companionNdx = 14;
   else
      throw std::runtime_error("Companion cost outside of expected bounds.");

   // Calculate ndx
   size_t ndx =
      roundNdx*actionTypeCount*cardIdCount*moneyCount*actionCount*companionCount +
      typeNdx*cardIdCount*moneyCount*actionCount*companionCount +
      cardNdx*moneyCount*actionCount*companionCount +
      moneyNdx*actionCount*companionCount +
      actionNdx*companionCount +
      companionNdx;

   // Final safety check
   if (ndx >= getNumActions())
      throw std::runtime_error("Action ndx computed to be outside of expected bounds.");

   return ndx;
}
