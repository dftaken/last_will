/*
 * Database.h
 *
 *  Created on: Oct 15, 2018
 *      Author: milites
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <assets/Action.h>

struct ActionRecord
{
   unsigned int w; // Wins
   unsigned int l; // Losses
};

class Database
{
public:
   ~Database();

   static Database& instance();

   bool loadDatabase(char*);
   void recordAction(bool,int,Action);
   ActionRecord *getActions();
   float computeScore(ActionRecord &);

private:
   Database();

   int fd;
   ActionRecord *actions;
};

#endif /* DATABASE_H_ */
