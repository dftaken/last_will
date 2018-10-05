/*
 * CardSpot.h
 *
 *  Created on: Sep 14, 2018
 *      Author: milites
 */

#ifndef CARDSPOT_H_
#define CARDSPOT_H_

#include <assets/cards/Card.h>

#include <vector>

class CardSpot : public Card::Location
{
public:
   typedef CardSpot* Ptr;

   CardSpot(std::string);
   ~CardSpot();

   // Card::Location Functions
   virtual std::string getName();
   virtual void releaseCard(Card::Ptr);
   virtual void releaseCard(Card&);
   virtual void releaseCard(CardIdType);

   bool hasBeenActivated();
   void activate();
   bool hasBeenExpensed();
   void expense();
   bool isOccupied();
   Card::Ptr getCard();
   void setCard(Card::Ptr);

   void reset();

   void executeEndOfRound();

private:
   std::string playerName;
   bool wasActivated;
   bool wasExpensed;
   Card::Ptr card;
};

typedef std::vector<CardSpot::Ptr> CardSpots;

#endif /* CARDSPOT_H_ */
