/*
 * BoardSpot.h
 *
 *  Created on: Sep 13, 2018
 *      Author: milites
 */

#ifndef BOARDSPOT_H_
#define BOARDSPOT_H_

#include <assets/cards/CardAttributes.h>
#include <assets/cards/Card.h>

#include <string>
#include <vector>

struct Spot
{
   enum Type {
      Invalid=-1,
      PropertyMarket=0,
      BoardExpansion,
      Opera,
      SingleDraw,
      Card
   };
   static std::string toString(Type);
};
typedef Spot::Type SpotType;

class BoardSpot : public Card::Location
{
public:
   typedef BoardSpot* Ptr;

   BoardSpot();
   BoardSpot(SpotType);
   BoardSpot(SpotType,CardDeckType);
   BoardSpot(SpotType,CardDeckType,CardDeckType,CardDeckType,
             CardDeckType,CardDeckType,CardDeckType,CardDeckType);
   ~BoardSpot();

   // Card Location Functions
   virtual std::string getName();
   virtual void releaseCard(Card::Ptr);
   virtual void releaseCard(Card&);
   virtual void releaseCard(CardIdType);

   void setCard(Card::Ptr);
   void executeEndOfRound();
   void reset();
   void releaseCard();

   void print();

   SpotType type;
   bool occupied;
   std::string player;
   Card::Ptr card;
   CardDeckTypes decks;
   // Deck to draw from for different rounds
};

typedef std::vector<BoardSpot> BoardSpotObjects;
typedef std::vector<BoardSpot::Ptr> BoardSpots;

#endif /* BOARDSPOT_H_ */
