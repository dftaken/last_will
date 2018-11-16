/*
 * Indexer.cpp
 *
 *  Created on: Oct 13, 2018
 *      Author: milites
 */

#include <database/Indexer.h>
#include <stdexcept>

/* Round Permutations
 * 1 - 7
 */
#define ROUND_COUNT 7
int Indexer::roundCount = ROUND_COUNT;

/* Action Type Permutations
 * 0 - Play
 * 1 - Upgrade
 * 2 - Expense
 * 3 - Activate
 * 4 - Sell
 * 5 - Discard
 */
#define ACTIONTYPE_COUNT 6
int Indexer::actionTypeCount = ACTIONTYPE_COUNT;

/* Card Id Permutations
 * Card Ids Farm_1 - Steward_S2
 * # = 111
 */
#define CARDID_COUNT 111
int Indexer::cardIdCount = CARDID_COUNT;

/* Money Cost Permutations
 * -8 - 30
 * For sell actions, money cost is multiplied by -1 since the
 * rest of the game uses negative cost to translate to money
 * going back to the player.
 */
#define MONEY_COUNT 39
int Indexer::moneyCount = MONEY_COUNT;

/* Action Count Permutations
 * 0 - 7
 */
#define ACTION_COUNT 8
int Indexer::actionCount = ACTION_COUNT;

/* Companion Cost Permutations
 * H (horse), D (dog), G (guest), C (chef)
 * None, H, D, G, C, GDC, GD, GC, DC, GG, CC
 * ONLY FOR 15: HH, DH, DD, DDD
 */
#define COMPANION_COUNT 11
int Indexer::companionCount = COMPANION_COUNT;
//int Indexer::companionCount = 15;

/*
 * Index Modifiers
 */
int Indexer::roundMod = ACTIONTYPE_COUNT * CARDID_COUNT * MONEY_COUNT * ACTION_COUNT * COMPANION_COUNT;
int Indexer::actionTypeMod = CARDID_COUNT * MONEY_COUNT * ACTION_COUNT * COMPANION_COUNT;
int Indexer::cardIdMod = MONEY_COUNT * ACTION_COUNT * COMPANION_COUNT;
int Indexer::moneyMod = ACTION_COUNT * COMPANION_COUNT;
int Indexer::actionMod = COMPANION_COUNT;
int Indexer::companionMod = 1;

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

void Indexer::convertActionNdx(
   size_t ndx,
   int &round,
   int &type,
   int &cardId,
   int &moneyCost,
   int &actionCost,
   int &horseCost,
   int &dogCost,
   int &chefCost,
   int &guestCost)
{
   round = ndx / roundMod;
   ndx -= round * roundMod;
   round += 1;

   type = ndx / actionTypeMod;
   ndx -= type * actionTypeMod;
   type += ActionType::Play;

   cardId = ndx / cardIdMod;
   ndx -= cardId * cardIdMod;
   cardId += CardId::Farm_1;

   moneyCost = ndx / moneyMod;
   ndx -= moneyCost * moneyMod;
   moneyCost -= 8;
   if (type == ActionType::Sell)
      moneyCost *= -1;

   actionCost = ndx / actionMod;
   ndx -= actionCost * actionMod;

   unsigned int companionNdx = ndx / companionMod;
   switch (companionNdx)
   {
      case 0:
         horseCost = 0;
         dogCost = 0;
         chefCost = 0;
         guestCost = 0;
         break;
      case 1:
         horseCost = 1;
         dogCost = 0;
         chefCost = 0;
         guestCost = 0;
         break;
      case 2:
         horseCost = 0;
         dogCost = 1;
         chefCost = 0;
         guestCost = 0;
         break;
      case 3:
         horseCost = 0;
         dogCost = 0;
         chefCost = 1;
         guestCost = 0;
         break;
      case 4:
         horseCost = 0;
         dogCost = 0;
         chefCost = 0;
         guestCost = 1;
         break;
      case 5:
         horseCost = 0;
         dogCost = 1;
         chefCost = 1;
         guestCost = 1;
         break;
      case 6:
         horseCost = 0;
         dogCost = 1;
         chefCost = 0;
         guestCost = 1;
         break;
      case 7:
         horseCost = 0;
         dogCost = 0;
         chefCost = 1;
         guestCost = 1;
         break;
      case 8:
         horseCost = 0;
         dogCost = 1;
         chefCost = 1;
         guestCost = 0;
         break;
      case 9:
         horseCost = 0;
         dogCost = 0;
         chefCost = 0;
         guestCost = 2;
         break;
      case 10:
         horseCost = 0;
         dogCost = 0;
         chefCost = 2;
         guestCost = 0;
         break;
      default:
         throw std::runtime_error("Indexer messed up somewhere converting ndx to action elements.");
         break;
   }
}

Indexes Indexer::getCardNdxs(int cardId)
{
   Indexes ndxs;
   for (size_t r = 0; r < roundCount; ++r)
   {
      for (size_t t = 0; t < actionTypeCount; ++t)
      {
         for (size_t m = 0; m < moneyCount; ++m)
         {
            for (size_t a = 0; a < actionCount; ++a)
            {
               for (size_t c = 0; c < companionCount; ++c)
               {
                  size_t ndx =
                     r * roundMod +
                     t * actionTypeMod +
                     cardId * cardIdMod +
                     m * moneyMod +
                     a * actionMod +
                     c * companionMod;
                  ndxs.push_back(ndx);
               }
            }
         }
      }
   }
   return ndxs;
}

size_t Indexer::getNumDraws()
{
   static size_t numDraws = 7*14*14*14*14;
   return numDraws;
}

size_t Indexer::getDrawNdx(
   int round,
   unsigned int propertyDraws,
   unsigned int eventDraws,
   unsigned int helperDraws,
   unsigned int companionDraws)
{
   if (round < 1 ||
       round > 7 ||
       propertyDraws > 13 ||
       eventDraws > 13 ||
       helperDraws > 13 ||
       companionDraws > 13)
      throw std::runtime_error("Invalid Draw ndx attributes");
   size_t ndx = (round-1)*14*14*14*14 + propertyDraws*14*14*14 + eventDraws*14*14 + helperDraws*14 + companionDraws;
   return ndx;
}
