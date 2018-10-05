/*
 * PropertyMarket.cpp
 *
 *  Created on: Sep 9, 2018
 *      Author: milites
 */

#include <assets/PropertyMarket.h>
#include <system_utils/SysUtils.h>

using namespace std;

PropertyMarket::PropertyMarket() :
   propertyAdjustments(),
   adjustments()
{
   adjustments.push_back(AdjustValue::Minus3);
   adjustments.push_back(AdjustValue::Minus2);
   adjustments.push_back(AdjustValue::Plus1);
   adjustments.push_back(AdjustValue::Plus3);

   reset();
}

PropertyMarket::~PropertyMarket()
{
   // Intentionally left blank
}

void PropertyMarket::reset()
{
   // Make copy of adjustments to pull from
   Adjustments tmp = adjustments;

   // Randomly assign adjustments to different property types
   for (size_t i = tmp.size(); i > 0; --i)
   {
      size_t ndx = SysUtils::getRandL() % i;
      switch (i)
      {
         case 4:
            propertyAdjustments[CardName::Farm] = tmp.at(ndx);
            break;
         case 3:
            propertyAdjustments[CardName::TownHouse] = tmp.at(ndx);
            break;
         case 2:
            propertyAdjustments[CardName::ManorHouse] = tmp.at(ndx);
            break;
         case 1:
            propertyAdjustments[CardName::Mansion] = tmp.at(ndx);
            break;
         default:
            break;
      }
      tmp.erase(tmp.begin()+ndx);
   }
}

int PropertyMarket::getPropertyAdjustment(CardNameType card)
{
   return propertyAdjustments[card];
}

PropertyAdjustments PropertyMarket::getPropertyAdjustments()
{
   return propertyAdjustments;
}

void PropertyMarket::setPropertyAdjustments(PropertyAdjustments adjusts)
{
   propertyAdjustments = adjusts;
}

Adjustments PropertyMarket::getAdjustments()
{
   return adjustments;
}
