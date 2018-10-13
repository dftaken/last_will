/*
 * PlayerBoard.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#include <assets/PlayerBoard.h>
#include <Logging.h>

PlayerBoard::PlayerBoard(std::string name):
   playerName(name),
   spots(7,NULL)
{
   for (size_t i = 0; i < spots.size(); ++i)
      spots[i] = new CardSpot(playerName);
}

PlayerBoard::~PlayerBoard()
{
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      delete (*itr);
   }
}

unsigned int PlayerBoard::getNumAvailSpots()
{
   unsigned int availCount = 0;
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      if (!((*itr)->isOccupied()))
      {
         availCount++;
      }
   }
   return availCount;
}

unsigned int PlayerBoard::getNumOccupiedSpots()
{
   unsigned int occupiedCount = 0;
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      if ((*itr)->isOccupied())
      {
         occupiedCount++;
      }
   }
   return occupiedCount;
}

CardSpots PlayerBoard::getAvailSpots()
{
   CardSpots avail;
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      if (!((*itr)->isOccupied()))
      {
         avail.push_back(*itr);
      }
   }
   return avail;
}

CardSpots PlayerBoard::getOccupiedSpots()
{
   CardSpots occupied;
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      if ((*itr)->isOccupied())
      {
         occupied.push_back(*itr);
      }
   }
   return occupied;
}

CardSpots PlayerBoard::getSpots()
{
   return spots;
}

Cards PlayerBoard::getCards()
{
   Cards cards;
   for (size_t i = 0; i < spots.size(); ++i)
   {
      if (spots[i]->isOccupied())
      {
         cards.push_back(spots[i]->getCard());
      }
   }
   return cards;
}

void PlayerBoard::placeCard(Card::Ptr card)
{
   for (size_t i = 0; i < spots.size(); ++i)
   {
      if (!(spots[i]->isOccupied()))
      {
         spots[i]->setCard(card);
         break;
      }
   }
}

void PlayerBoard::extend()
{
   spots.push_back(new CardSpot(playerName));
}

void PlayerBoard::executeEndOfRound()
{
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      (*itr)->executeEndOfRound();
   }
}

void PlayerBoard::reset()
{
   for (CardSpots::iterator itr = spots.begin(); itr != spots.end(); ++itr)
   {
      delete (*itr);
   }
   spots.clear();

   for (int i = 0; i < 7; ++i)
   {
      spots.push_back(new CardSpot(playerName));
   }
}

void PlayerBoard::print()
{
   printf("Board State:\n");
   printf("# Available Spots: %u\n",getNumAvailSpots());
   printf("# Occupied Spots: %u\n",getNumOccupiedSpots());
   printf("Occupied Spots (Card Id, Occupied, Activated, Expensed):\n");
   CardSpots spots = getOccupiedSpots();
   for (CardSpots::iterator spot = spots.begin(); spot != spots.end(); ++spot)
   {
      printf("   \"%s\", %d, %d, %d\n",
             CardId::toString((*spot)->getCard()->id).c_str(),
             (*spot)->isOccupied(),
             (*spot)->hasBeenActivated(),
             (*spot)->hasBeenExpensed());
   }
}
