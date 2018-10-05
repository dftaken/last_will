/*
 * PlayerHand.h
 *
 *  Created on: Sep 14, 2018
 *      Author: milites
 */

#ifndef PLAYERHAND_H_
#define PLAYERHAND_H_

#include <assets/cards/Card.h>

class PlayerHand : public Card::Location
{
public:
   PlayerHand(std::string);
   ~PlayerHand();

   // Card::Location Functions
   virtual std::string getName();
   virtual void releaseCard(Card::Ptr);
   virtual void releaseCard(Card&);
   virtual void releaseCard(CardIdType);
   void releaseCards();

   void addCard(Card::Ptr);
   Card::Ptr removeCard(unsigned int);
   Card::Ptr lookAtCard(unsigned int);
   Cards getCards();
   unsigned int getSize();

   void reset();

   void print();

private:
   std::string playerName;
   Cards cards;
};

#endif /* PLAYERHAND_H_ */
