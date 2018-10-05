/*
 * PlayerHand.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <assets/PlayerHand.h>

PlayerHand::PlayerHand(std::string name) :
   playerName(name),
   cards()
{
   // Intentionally left blank
}

PlayerHand::~PlayerHand()
{
   // Intentionally left blank
}

std::string PlayerHand::getName()
{
   return playerName + "'s Hand";
}

void PlayerHand::releaseCard(Card::Ptr ptr)
{
   for (Cards::iterator itr = cards.begin(); itr != cards.end(); ++itr)
   {
      if ((*itr) != NULL && (*itr) == ptr)
      {
         (*itr)->location = NULL;
         cards.erase(itr);
         break;
      }
   }
}

void PlayerHand::releaseCard(Card &ref)
{
   releaseCard(&ref);
}

void PlayerHand::releaseCard(CardIdType id)
{
   for (Cards::iterator itr = cards.begin(); itr != cards.end(); ++itr)
   {
      if ((*itr) != NULL && (*itr)->id == id)
      {
         (*itr)->location = NULL;
         cards.erase(itr);
         break;
      }
   }
}

void PlayerHand::releaseCards()
{
   for (Cards::iterator itr = cards.begin(); itr != cards.end();)
   {
      (*itr)->location = NULL;
      itr = cards.erase(itr);
   }
}

void PlayerHand::addCard(Card::Ptr card)
{
   if (card != NULL)
   {
      if (card->location != NULL)
      {
         card->location->releaseCard(card);
      }

      card->location = this;
      cards.push_back(card);
   }
}

Cards PlayerHand::getCards()
{
   return cards;
}

unsigned int PlayerHand::getSize()
{
   return cards.size();
}

void PlayerHand::reset()
{
   releaseCards();
}

void PlayerHand::print()
{
   printf("Hand State:\n");
   printf("# of cards: %lu\n",cards.size());
   printf("Cards:\n");
   for (Cards::iterator itr = cards.begin(); itr != cards.end(); ++itr)
   {
      printf("   %s\n",CardId::toString((*itr)->id).c_str());
   }
}
