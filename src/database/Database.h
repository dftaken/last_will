/*
 * Database.h
 *
 *  Created on: Oct 15, 2018
 *      Author: milites
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <assets/Action.h>

struct Record
{
   unsigned int w; // Wins
   unsigned int l; // Losses

   bool operator<(const Record &rhs);
   bool operator>(const Record &rhs);
};

class Database
{
public:
   ~Database();

   static Database& instance();

   bool loadDatabase(char*);
   void recordAction(bool,int,Action);
   Record *getActions();
   float computeScore(Record &);

   bool loadDrawDB(char*);
   void recordDraw(bool,int,unsigned int, unsigned int, unsigned int, unsigned int);
   Record *getDraws();

private:
   Database();

   int fd;
   int drawFd;
   Record *actions;
   Record *draws;
};

#endif /* DATABASE_H_ */
