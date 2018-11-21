/*
 * Logger.h
 *
 *  Created on: Oct 9, 2018
 *      Author: milites
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <assets/Action.h>
#include <assets/Plan.h>
#include <assets/BoardSpot.h>
#include <string>

class Logger
{
public:
   ~Logger();

   static Logger& instance();

   bool openActionFile(char*);
   void recordAction(long unsigned int, bool, std::string, int, Action);

   bool openPlanFile(char*);
   void recordPlan(
      long unsigned int game,
      bool won,
      std::string player,
      int round,
      Plan::Ptr plan,
      Cards cards);

   bool openErrandFile(char*);
   void recordErrand(
      long unsigned int game,
      bool won,
      std::string player,
      int round,
      SpotType type,
      Card::Ptr card);

   bool openStatsFile(const char*,std::map<std::string,int>);
   void recordStats(
      long unsigned int,
      int startingMoney,
      std::string winner,
      std::map<std::string,int> players);

protected:
   Logger();

   void writeActionHeader();
   FILE *actionFile;

   void writePlanHeader();
   FILE *planFile;

   void writeErrandHeader();
   FILE *errandFile;

   void writeStatsHeader(std::map<std::string,int>);
   FILE *statsFile;
};



#endif /* LOGGER_H_ */
