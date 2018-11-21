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

using namespace std;

Logger::Logger() :
   actionFile(NULL),
   planFile(NULL),
   errandFile(NULL),
   statsFile(NULL)
{
   // Intentionally left blank
}

Logger::~Logger()
{
   if (actionFile != NULL)
   {
      fclose(actionFile);
   }

   if (planFile != NULL)
   {
      fclose(planFile);
   }

   if (errandFile != NULL)
   {
      fclose(errandFile);
   }

   if (statsFile != NULL)
   {
      fclose(statsFile);
   }
}

Logger& Logger::instance()
{
   static Logger logger;
   return logger;
}

/*
 * Action File
 */

bool Logger::openActionFile(char *fname)
{
   if (actionFile != NULL)
   {
      fclose(actionFile);
   }

   FILE *outputFile = fopen(fname,"w+");
   if (outputFile != NULL)
   {
      actionFile = outputFile;
      writeActionHeader();
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
   if (actionFile == NULL)
      openActionFile("lw_actions.csv");

   // Game id
   fprintf(actionFile,"%lu,",game);

   // Win/Loss
   if (won)
      fprintf(actionFile,"W,");
   else
      fprintf(actionFile,"L,");

   // Player name, Round, Action Type
   fprintf(actionFile,"%s,%d,%s,",
           name.c_str(),
           round,
           ActionType::toString(action.type).c_str());

   // Card Id
   if (action.card != NULL)
      fprintf(actionFile,"%s",CardId::toString(action.card->id).c_str());
   else
      fprintf(actionFile,"NONE");

   // Card Resources
   for (int i = Resource::Money; i <= Resource::ActionAnd2Money; ++i)
      fprintf(actionFile,",%d",action.cost[ResourceType(i)]);

   fprintf(actionFile,"\n");
}

void Logger::writeActionHeader()
{
   // Print the Enumerations

   // Print the column headers
   fprintf(actionFile,"GameId,W/L,PlayerName,Round#,Type,CardId,");
   for (int i = Resource::Money; i <= Resource::ActionAnd2Money; ++i)
      fprintf(actionFile,"%s,",Resource::toString(ResourceType(i)).c_str());
   fprintf(actionFile,"\n");
}

/*
 * Plan File
 */

bool Logger::openPlanFile(char *fname)
{
   if (planFile != NULL)
   {
      fclose(planFile);
   }

   FILE *outputFile = fopen(fname,"w+");
   if (outputFile != NULL)
   {
      planFile = outputFile;
      writePlanHeader();
   }
   else
   {
      fprintf(stderr,"Failed to open output file \"%s\"\n",fname);
      throw std::runtime_error("Failed to open output file.");
   }

   return true;
}

void Logger::recordPlan(
   long unsigned int game,
   bool won,
   std::string name,
   int round,
   Plan::Ptr plan,
   Cards cards)
{
   if (planFile == NULL)
      openPlanFile("lw_plans.csv");

   fprintf(planFile,"%lu,%c,%s,%d,%d,%d,%d,%d",
           game,
           (won ? 'W' : 'L'),
           name.c_str(),
           round,
           plan->priority,
           plan->cards,
           plan->errands,
           plan->actions);
   for (size_t i = 0; i < cards.size(); ++i)
   {
      fprintf(planFile,",%s",CardId::toString(cards.at(i)->id).c_str());
   }
   fprintf(planFile,"\n");
}

void Logger::writePlanHeader()
{
   // Print the column headers
   fprintf(planFile,"GameId,W/L,PlayerName,Round#,Priority,#Cards,#Errands,#Actions,CardsDrawn\n");
}

/*
 * Errand File
 */

bool Logger::openErrandFile(char *fname)
{
   if (errandFile != NULL)
   {
      fclose(errandFile);
   }

   FILE *outputFile = fopen(fname,"w+");
   if (outputFile != NULL)
   {
      errandFile = outputFile;
      writeErrandHeader();
   }
   else
   {
      fprintf(stderr,"Failed to open output errand file \"%s\"\n",fname);
      throw std::runtime_error("Failed to open output errand file.");
   }

   return true;
}

void Logger::recordErrand(
   long unsigned int game,
   bool won,
   std::string name,
   int round,
   SpotType type,
   Card::Ptr card)
{
   if (errandFile == NULL)
      openErrandFile("lw_errands.csv");

   std::string cardId = card != NULL ? CardId::toString(card->id) : std::string("N/A");
   fprintf(errandFile,"%lu,%c,%s,%d,%s,%s\n",
           game,
           (won ? 'W' : 'L'),
           name.c_str(),
           round,
           Spot::toString(type).c_str(),
           cardId.c_str());
}

void Logger::writeErrandHeader()
{
   // Print the column headers
   fprintf(errandFile,"GameId,W/L,PlayerName,Round#,Type,Card\n");
}

/*
 * Stats File
 */

bool Logger::openStatsFile(const char *fname, map<string,int> players)
{
   if (statsFile != NULL)
   {
      fclose(statsFile);
   }

   FILE *outputFile = fopen(fname,"w+");
   if (outputFile != NULL)
   {
      statsFile = outputFile;
      writeStatsHeader(players);
   }
   else
   {
      fprintf(stderr,"Failed to open output stats file \"%s\"\n",fname);
      throw std::runtime_error("Failed to open output stats file.");
   }

   return true;
}

void Logger::recordStats(
   long unsigned int gameId,
   int startingMoney,
   string winner,
   map<string,int> players)
{
   if (statsFile == NULL)
      openStatsFile("lw_stats.csv",players);

   fprintf(statsFile,"%lu,%d,%s",gameId,startingMoney,winner.c_str());
   for (map<string,int>::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      fprintf(statsFile,",%d",itr->second);
   }
   fprintf(statsFile,"\n");
}

void Logger::writeStatsHeader(map<string,int> players)
{
   // Print the column headers
   fprintf(statsFile,"GameId,StartingMoney,WinningPlayerName");
   for (map<string,int>::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      fprintf(statsFile,",%s Ending Money",itr->first.c_str());
   }
   fprintf(statsFile,"\n");
}
