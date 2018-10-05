/*
 * Card.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: milites
 */

#include <assets/cards/Card.h>

Card::Card() :
   id(CardId::Invalid),
   name(CardName::Invalid),
   group(CardGroup::Invalid),
   type(CardType::Invalid),
   deck(CardDeck::Invalid),
   strName(""),
   valueLevels(),
   curValue(0),
   upgradeLevels(),
   curLevel(0),
   additiveUpgrades(false),
   expenseCost(0),
   effect(Effect::None),
   crowns(0),
   location(NULL)
{
   // Intentionally left blank
}

Card::~Card()
{
   // Intentionally left blank
}

void Card::depreciate()
{
   if (group == CardGroup::Property)
   {
      if (valueLevels.size() > 0 && curValue < (valueLevels.size()-1))
      {
         curValue++;
      }
   }
}

void Card::addValueLvl(ResourceType resource, int value)
{
   valueLevels.push_back(ValueLevel(resource,value));
}

void Card::addUpgradeLvl(ResourceType costType, int expense)
{
   upgradeLevels.push_back(UpgradeLevel(costType,expense));
}

void Card::reset()
{
   curValue = 0;
   curLevel = 0;
}

Card::Location::Location()
{
   // Intentionally left blank
}

Card::Location::~Location()
{
   // Intentionally left blank
}
