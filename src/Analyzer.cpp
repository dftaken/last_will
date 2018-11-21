/*
 * Analyzer.cpp
 *
 *  Created on: Oct 19, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <database/Database.h>
#include <database/Indexer.h>

float calcRatio(Record ar)
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
   bool singlePass = false;
   if (argc > 1)
      Database::instance().loadDatabase(argv[1]);
   if (argc > 2)
      singlePass = true;

   Record *actions = Database::instance().getActions();

   fprintf(stderr,"Starting %s, opening \"%s\"\n",argv[0],argv[1]);
   float highRatio = -1.0f;
   size_t highNdx = 0;
   while (true)
   {
      for (size_t i = 0; i < Indexer::getNumActions(); ++i)
      {
         Record ari = actions[i];
         Record arh = actions[highNdx];
         long unsigned int sumi = ari.w + ari.l;
         long unsigned int sumh = arh.w + arh.l;
         float ratio = calcRatio(ari);
         float high = calcRatio(arh);
         if (ratio > high ||
             (ratio == high && sumi > sumh))
         {
            fprintf(stderr,"Action[%lu] ( %lu , %lu | %.03f ) > Action[%lu] ( %lu , %lu | %.03f )\n",
                   i,ari.w,ari.l,ratio,
                   highNdx,arh.w,arh.l,high);
            highNdx = i;
         }
         if (singlePass && (ari.w > 0 || ari.l > 0))
            fprintf(stderr,"ratio of %lu = %.03f (W / L = %lu / %lu)\n",i,ratio,ari.w,ari.l);
      }
      if (singlePass)
         break;
   }

   return 0;
}
