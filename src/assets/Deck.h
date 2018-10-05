/*
 * Deck.h
 *
 *  Created on: Sep 8, 2018
 *      Author: milites
 */

#ifndef DECK_H_
#define DECK_H_

#include <assets/cards/Card.h>
#include <assets/cards/CardAttributes.h>

class Deck : public Card::Location
{
public:
   typedef Deck* Ptr;

   Deck(CardDeckType);
   ~Deck();

   /*
    * Deck Functions
    */
   // Takes back all cards that belong to this deck and shuffles
   // them into the draw stack
   void reset();
   // Shuffle the draw pile
   void shuffle();
   // Draw a card
   Card::Ptr draw();
   // Discard a card
   void discard(Card::Ptr);
   // Get number of cards in draw pile
   size_t getDrawSize();
   // Get number of cards in discard pile
   size_t getDiscardSize();

   /*
    * Location Functions
    */
   virtual std::string getName();
   virtual void releaseCard(Card::Ptr);
   virtual void releaseCard(Card&);
   virtual void releaseCard(CardIdType);

private:
   CardDeckType type;
   Cards discardPile;
   Cards drawPile;
};

#endif /* DECK_H_ */
