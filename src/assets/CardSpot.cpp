/*
 * CardSpot.cpp
 *
 *  Created on: Sep 14, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <assets/CardSpot.h>

#include <stdexcept>

using namespace std;

CardSpot::CardSpot(string name) :
   playerName(name),
   wasActivated(false),
   wasExpensed(false),
   card(NULL)
{
   // Intentionally left blank
}

CardSpot::~CardSpot()
{
   releaseCard(card);
}

string CardSpot::getName()
{
   return playerName + string("'s Board");
}

void CardSpot::releaseCard(Card::Ptr ptr)
{
   if (ptr != NULL && ptr == card)
   {
      card->location = NULL;
      card = NULL;
   }
}

void CardSpot::releaseCard(Card &ref)
{
   releaseCard(&ref);
}

void CardSpot::releaseCard(CardIdType id)
{
   if (card != NULL && card->id == id)
   {
      releaseCard(card);
   }
}

bool CardSpot::hasBeenActivated()
{
   return wasActivated;
}

void CardSpot::activate()
{
   if (wasActivated)
   {
      throw std::runtime_error(playerName +
                               " attempted to activate a CardSpot again.");
   }
   else
   {
      wasActivated = true;
   }
}

bool CardSpot::hasBeenExpensed()
{
   return wasExpensed;
}

void CardSpot::expense()
{
   if (wasExpensed)
   {
      throw std::runtime_error(playerName +
                               " attempted to expense a CardSpot again.");
   }
   else
   {
      wasExpensed = true;
   }
}

bool CardSpot::isOccupied()
{
   return (card != NULL);
}

Card::Ptr CardSpot::getCard()
{
   return card;
}

void CardSpot::setCard(Card::Ptr ptr)
{
   if (card != NULL)
   {
      throw std::runtime_error(playerName + " tried to place a card on an occupied spot");
   }
   else if (ptr != NULL)
   {
      if (ptr->location != NULL)
         ptr->location->releaseCard(ptr);
      card = ptr;
      card->location = this;
   }
}

void CardSpot::reset()
{
   wasActivated = false;
   wasExpensed = false;
   releaseCard(card);
}

void CardSpot::executeEndOfRound()
{
   wasActivated = false;
   wasExpensed = false;
   if (card != NULL && card->group == CardGroup::Property)
   {
      card->depreciate();
   }
}
