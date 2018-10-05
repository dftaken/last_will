/*
 * HousingMarket.h
 *
 *  Created on: Sep 9, 2018
 *      Author: milites
 */

#ifndef PROPERTYMARKET_H_
#define PROPERTYMARKET_H_

#include <assets/cards/CardAttributes.h>

#include <map>
#include <vector>

struct AdjustValue
{
   enum Type {
      Minus3=-3,
      Minus2=-2,
      Plus1=1,
      Plus3=3
   };
};
typedef AdjustValue::Type AdjustValueType;
typedef std::vector<AdjustValueType> Adjustments;
typedef std::map<CardNameType,AdjustValueType> PropertyAdjustments;

class PropertyMarket
{
public:
   PropertyMarket();
   ~PropertyMarket();

   void reset();

   int getPropertyAdjustment(CardNameType);
   PropertyAdjustments getPropertyAdjustments();
   void setPropertyAdjustments(PropertyAdjustments);

   Adjustments getAdjustments();

protected:
   PropertyAdjustments propertyAdjustments;
   Adjustments adjustments;
};

#endif /* PROPERTYMARKET_H_ */
