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
   actions(NULL)
{
   actions = Database::instance().getActions();
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
