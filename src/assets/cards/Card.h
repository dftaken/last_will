/*
 * Card.h
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#ifndef CARD_H_
#define CARD_H_

#include <string>
#include <vector>
#include <assets/cards/CardAttributes.h>

class Card
{
public:
   // Typedefs
   typedef Card* Ptr;

   // Inheritable property for card containers
   class Location
   {
   public:
      typedef Location* Ptr;

      virtual ~Location();

      virtual std::string getName()=0;

      // The location is expected to NULL out the card's
      // location when it releases the card.
      virtual void releaseCard(Card::Ptr)=0;
      virtual void releaseCard(Card&)=0;
      virtual void releaseCard(CardIdType)=0;
   protected:
      Location();
   };

   // Constructor/Destructor
   Card();
   virtual ~Card();

   /*
    * Common Functions
    */
   void depreciate();

   /*
    *  Common Attributes
    */
   CardIdType id;
   CardNameType name;
   CardGroupType group;
   CardTypeType type;
   CardDeckType deck;

   std::string strName;

   /*
    * Property/Event Attributes
    */
   // Value reflects the card's cost, units vary based on type
   // Property = $
   // Event = Actions
   ValueLevels valueLevels;
   unsigned int curValue;
   // Upgrade reflects how much is expensed by the card.
   // In both cases, the cost var reflects the resource required
   // to access that upgrade level (Invalid is the given/base
   // level and will always be [0]). For Property type cards,
   // the expense of a level reflects the total current value of
   // the property. For Event type cards, the expense of a level
   // reflects an additional expenditure (sum of the applicable
   // upgrade levels gives total expense).
   // For events that can consume multiple actions to increase
   // the expense, there is an implicit linkage between value
   // levels and upgrade levels through matching indexes.
   UpgradeLevels upgradeLevels;
   unsigned int curLevel;
   // additiveUpgrades signals that the upgrade levels are
   // additive by nature. As upgrades are paid for, the
   // corresponding value is added to the base expense.
   bool additiveUpgrades;

   // expenseCost reflects the number of actions required to
   // "expense" a card (activate it's expense/upkeep)
   // This is not relevant for event cards.
   unsigned int expenseCost;

   // Effect data
   EffectType effect;

   // Crown count, only for special cards
   int crowns;

   // Data Loading Functions
   void addValueLvl(ResourceType,int);
   void addUpgradeLvl(ResourceType,int);

   // Reset any upgrades to return the card to a fresh state
   void reset();

   Location::Ptr location;
};

typedef std::vector<Card::Ptr> Cards;

#endif /* CARD_H_ */
