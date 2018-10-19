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
#include <string>

class Logger
{
public:
   ~Logger();

   static Logger& instance();

   bool openOutputFile(char*);

   void recordAction(long unsigned int, bool, std::string, int, Action);

protected:
   Logger();

   void writeOutputHeader();
   FILE *output;
};



#endif /* LOGGER_H_ */
