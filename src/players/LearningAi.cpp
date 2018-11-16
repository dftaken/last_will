/*
 * LearningAi.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: milites
 */

#include <players/LearningAi.h>
#include <database/Indexer.h>

using namespace std;

LearningAi::LearningAi(string myName) :
   RandomAi(myName),
   actions(NULL),
   draws(NULL)
{
   actions = Database::instance().getActions();
   draws = Database::instance().getDraws();
}

LearningAi::~LearningAi()
{
   // Intentionally left blank
}

Cards LearningAi::discardCards(unsigned int numToDiscard)
{
   Cards toDiscard;

   Cards myCards = myState->hand.getCards();
   sortCardsByScore(myCards);

   Cards::reverse_iterator ritr = myCards.rbegin();
   for (unsigned int i = numToDiscard; i > 0 && ritr != myCards.rend(); --i)
   {
      toDiscard.push_back(*ritr);
      ritr++;
   }

   return toDiscard;
}

Cards LearningAi::depreciateThreeProperties()
{
   // Determine eligible properties
   Cards properties;
   vector<unsigned int> curValNdx;
   Cards boardCards = myState->board.getCards();
   for (size_t i = 0; i < boardCards.size(); ++i)
   {
      if (boardCards[i]->group == CardGroup::Property &&
          boardCards[i]->name != CardName::Farm &&
          boardCards[i]->curValue < (boardCards[i]->valueLevels.size() - 1))
      {
         properties.push_back(boardCards[i]);
         curValNdx.push_back(boardCards[i]->curValue);
      }
   }

   // Pick the top 3 depreciations
   Cards choices;
   if (properties.size() > 0)
   {
      for (int count = 0; count < 3; ++count)
      {
         int maxDep = 0;
         int maxDepNdx = -1;
         for (size_t i = 0; i < properties.size(); ++i)
         {
            unsigned int valNdx = curValNdx[i];
            unsigned int nextNdx = valNdx+1;
            int dep = properties[i]->valueLevels[valNdx].value - properties[i]->valueLevels[nextNdx].value;
            if (nextNdx < properties[i]->valueLevels.size() &&
                dep > maxDep)
            {
               maxDep = dep;
               maxDepNdx = i;
            }
         }
         if (maxDepNdx >= 0)
         {
            choices.push_back(properties[maxDepNdx]);
            curValNdx[maxDepNdx]++;
         }
      }
   }

   return choices;
}

Cards LearningAi::placeTwoDogsHorses()
{
   // Determine eligible properties
   Cards properties;
   Cards boardCards = myState->board.getCards();
   for (size_t i = 0; i < boardCards.size(); ++i)
   {
      if (boardCards[i]->name == CardName::Farm)
      {
         unsigned int nextUpgrade = boardCards[i]->curLevel + 1;
         if (nextUpgrade < boardCards[i]->upgradeLevels.size())
         {
            ResourceType cost = boardCards[i]->upgradeLevels[nextUpgrade].cost;
            if (myState->resources[cost] > 0)
            {
               properties.push_back(boardCards[i]);
            }
         }
      }
   }

   // Pick 2 properties
   Cards choices;
   sortCardsByScore(properties);
   if (properties.size() > 0)
   {
      for (size_t i = 0; i < properties.size(); ++i)
      {
         if (choices.size() == 2)
            break;

         Card::Ptr card = properties[i];
         unsigned int nextUpgrade = card->curLevel + 1;
         ResourceType cost = card->upgradeLevels[nextUpgrade].cost;
         if (myState->resources[cost] > 0)
         {
            choices.push_back(card);
            nextUpgrade++;
            if (choices.size() == 2)
               break;

            if (nextUpgrade < card->upgradeLevels.size())
            {
               ResourceType nextCost = card->upgradeLevels[nextUpgrade].cost;
               if (cost == nextCost && myState->resources[cost] > 1)
               {
                  choices.push_back(card);
               }
               else if (cost != nextCost && myState->resources[cost] > 0)
               {
                  choices.push_back(card);
               }
            }
         }
      }
   }

   return choices;
}

CardDeckTypes LearningAi::chooseCardDraws(int numDraws, bool hardLimit)
{
   CardDeckTypes choices;
   if (!hardLimit)
      choices = useAllCardDrawEffects(numDraws);

   unsigned int p, e, h, c;
   getBestDraws(numDraws,p,e,h,c);
   for (int i = 0; i < p; i++)
      choices.push_back(CardDeck::Properties);
   for (int i = 0; i < e; i++)
      choices.push_back(CardDeck::Events);
   for (int i = 0; i < h; i++)
      choices.push_back(CardDeck::HelpersExpenses);
   for (int i = 0; i < c; i++)
      choices.push_back(CardDeck::Companions);
   return choices;
}

//void LearningAi::executeActions()
//{
//   Actions options = gameboard->getAvailableActions(this);
//   for (Actions::iterator itr = options.begin(); itr != options.end();)
//   {
//      if (itr->type == ActionType::Discard)
//         itr = options.erase(itr);
//      else
//         ++itr;
//   }
//
//   while (options.size() > 0)
//   {
//      size_t bestNdx = -1;
//      if (!gameboard->processAction(this,options[ndx]))
//      {
//         throw std::runtime_error(
//            "Something went wrong trying to process an action.");
//      }
//
//      // Get new set of possible actions and cull out discard options
//      options = gameboard->getAvailableActions(this);
//      for (Actions::iterator itr = options.begin(); itr != options.end();)
//      {
//         if (itr->type == ActionType::Discard)
//            itr = options.erase(itr);
//         else
//            ++itr;
//      }
//   }
//}

void LearningAi::sortCardsByScore(Cards &cards)
{
   Cards tmp = cards;
   cards.clear();

   vector<float> scores;
   for (Cards::iterator itr = tmp.begin(); itr != tmp.end(); ++itr)
   {
      scores.push_back(getCardScore(*itr));
   }

   while (tmp.size() > 0)
   {
      float maxScore = -1.0f;
      size_t maxNdx = 0;
      for (size_t i = 0; i < scores.size(); ++i)
      {
         if (scores[i] > maxScore)
         {
            maxScore = scores[i];
            maxNdx = i;
         }
      }

      cards.push_back(tmp.at(maxNdx));
      tmp.erase(tmp.begin()+maxNdx);
      scores.erase(scores.begin()+maxNdx);
   }
}

float LearningAi::getCardScore(Card *card)
{
   float score = -1.0f;
   if (card != NULL)
   {
      Indexes indexes = Indexer::getCardNdxs(card->id);
      int count = 0;
      float runningSum = 0.0f;
      for (Indexes::iterator ndx = indexes.begin(); ndx != indexes.end(); ++ndx)
      {
         if (actions[*ndx].l != 0 || actions[*ndx].w != 0)
         {
            count++;
            runningSum += Database::instance().computeScore(actions[*ndx]);
         }
      }

      if (count > 0)
         score = runningSum / (float)count;
   }
   return score;
}

void LearningAi::getBestDraws(
   int num,
   unsigned int &p,
   unsigned int &e,
   unsigned int &h,
   unsigned int &c)
{
   if (num <= 0)
   {
      p = 0; e = 0; h = 0; c = 0;
   }
   else
   {
      size_t maxNdx = 1;
      p = 0; e = 0; h = 0; c = 1;
      for (int P = 0; P <= num; ++P)
      {
         for (int E = 0; E <= (num-P); ++E)
         {
            for (int H = 0; H <= (num-P-E); ++H)
            {
               for (int C = 0; C <= (num-P-E-H); ++C)
               {
                  if ((P+E+H+C) == num)
                  {
                     size_t ndx = Indexer::getDrawNdx(gameboard->getRound(),P,E,H,C);
                     if (draws[ndx] > draws[maxNdx])
                     {
                        maxNdx = ndx;
                        p = P;
                        e = E;
                        h = H;
                        c = C;
                     }
                  }
               }
            }
         }
      }
   }
}
