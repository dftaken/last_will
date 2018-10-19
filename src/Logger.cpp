/*
 * Logger.cpp
 *
 *  Created on: Oct 5, 2018
 *      Author: milites
 */

#include <Logger.h>
#include <stdlib.h>
#include <stdexcept>
#include <assets/cards/CardAttributes.h>

Logger::Logger() :
   output(NULL)
{
   // Intentionally left blank
}

Logger::~Logger()
{
   if (output != NULL)
   {
      fclose(output);
   }
}

Logger& Logger::instance()
{
   static Logger logger;
   return logger;
}

bool Logger::openOutputFile(char *fname)
{
   if (output != NULL)
   {
      fclose(output);
   }

   FILE *outputFile = fopen(fname,"w+");
   if (outputFile != NULL)
   {
      output = outputFile;
      writeOutputHeader();
   }
   else
   {
      fprintf(stderr,"Failed to open output file \"%s\"\n",fname);
      throw std::runtime_error("Failed to open output file.");
   }

   return true;
}

void Logger::recordAction(
   long unsigned int game,
   bool won,
   std::string name,
   int round,
   Action action)
{
   if (output == NULL)
      openOutputFile("last_will.out");

   // Game id
   fprintf(output,"%lu,",game);

   // Win/Loss
   if (won)
      fprintf(output,"W,");
   else
      fprintf(output,"L,");

   // Player name, Round, Action Type
   fprintf(output,"%s,%d,%s,",
           name.c_str(),
           round,
           ActionType::toString(action.type).c_str());

   // Card Id
   if (action.card != NULL)
      fprintf(output,"%s",CardId::toString(action.card->id).c_str());
   else
      fprintf(output,"NONE");

   // Card Resources
   for (int i = Resource::Money; i <= Resource::ActionAnd2Money; ++i)
      fprintf(output,",%d",action.cost[ResourceType(i)]);

   fprintf(output,"\n");
}

void Logger::writeOutputHeader()
{
   // Print the Enumerations

   // Print the column headers
   fprintf(output,"GameId,W/L,PlayerName,Round#,Type,CardId,");
   for (int i = Resource::Money; i <= Resource::ActionAnd2Money; ++i)
      fprintf(output,"%s,",Resource::toString(ResourceType(i)).c_str());
   fprintf(output,"\n");
}
