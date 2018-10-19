/*
 * Analyzer.cpp
 *
 *  Created on: Oct 19, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <database/Database.h>
#include <database/Indexer.h>

float calcRatio(ActionRecord ar)
{
   float ratio = 0.0f;
   if (ar.w + ar.l > 0)
   {
      if (ar.w >= ar.l)
      {
         ratio = ((float)(ar.w - ar.l))/((float)(ar.w + ar.l));
      }
      else
      {
         ratio = -1.0f * ((float)(ar.l - ar.w))/((float)(ar.w + ar.l));
      }
   }
   return ratio;
}

int main(int argc, char **argv)
{
   ActionRecord *actions = Database::instance().getActions();

   printf("Starting %s\n",argv[0]);
   float highRatio = -1.0f;
   size_t highNdx = 0;
   while (true)
   {
      for (size_t i = 0; i < Indexer::getNumActions(); ++i)
      {
         ActionRecord ari = actions[i];
         ActionRecord arh = actions[highNdx];
         long unsigned int sumi = ari.w + ari.l;
         long unsigned int sumh = arh.w + arh.l;
         float ratio = calcRatio(ari);
         float high = calcRatio(arh);
         if (ratio > high ||
             (ratio == high && sumi > sumh))
         {
            printf("Action[%lu] ( %lu , %lu | %.03f ) > Action[%lu] ( %lu , %lu | %.03f )\n",
                   i,ari.w,ari.l,ratio,
                   highNdx,arh.w,arh.l,high);
            highNdx = i;
         }
      }
   }

   return 0;
}
