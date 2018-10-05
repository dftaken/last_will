/*
 * PlayerBoard.h
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#ifndef PLAYERBOARD_H_
#define PLAYERBOARD_H_

#include <assets/CardSpot.h>

#include <string>

class PlayerBoard
{
public:
   PlayerBoard(std::string);
   ~PlayerBoard();

   unsigned int getNumAvailSpots();
   unsigned int getNumOccupiedSpots();
   CardSpots getAvailSpots();
   CardSpots getOccupiedSpots();
   CardSpots getSpots();
   Cards getCards();

   void placeCard(Card::Ptr);

   void extend();

   void executeEndOfRound();

   void reset();

   void print();

protected:
   std::string playerName;
   CardSpots spots;
};

#endif /* PLAYERBOARD_H_ */
