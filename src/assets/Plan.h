/*
 * Plan.h
 *
 *  Created on: Sep 8, 2018
 *      Author: milites
 */

#ifndef PLAN_H_
#define PLAN_H_

#include <string>
#include <vector>

struct Plan
{
   typedef Plan* Ptr;

   class Location
   {
   public:
      typedef Location* Ptr;
      virtual ~Location() {};

      virtual std::string getName()=0;
      virtual void releasePlan()=0;

   protected:
      Location() {};
   };

   const int priority; // first == lowest
   const int cards; // # of cards to draw
   const int errands; // # of helpers to place
   const int actions; // # of actions to spend
   Location::Ptr location; // Occupying player

   Plan(int p, int c, int e, int a) :
      priority(p),
      cards(c),
      errands(e),
      actions(a),
      location(NULL)
   {};

   std::string getOwnersName()
   {
      if (location != NULL)
         return location->getName();
      else
         return std::string("");
   }
};
typedef std::vector<Plan::Ptr> Plans;

#endif /* PLAN_H_ */
