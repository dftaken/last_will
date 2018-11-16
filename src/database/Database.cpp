/*
 * Database.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: milites
 */

#include <database/Database.h>
#include <database/Indexer.h>

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

float computeScore(unsigned int w, unsigned int l)
{
   float score = 0.0f;
   if (w != 0 || l != 0)
   {
      if (w >= l)
      {
         score = ((float)(w - l)) / (float)(w + l);
      }
      else
      {
         score = ((float)(l - w)) / (float)(w + l);
         score *= -1.0f;
      }
   }
   return score;
}

bool Record::operator <(const Record &rhs)
{
   float ls = computeScore(w,l);
   float rs = computeScore(rhs.w,rhs.l);
   if (ls < rs)
   {
      return true;
   }
   else if (ls == rs)
   {
      long unsigned int ltotal = w+l;
      long unsigned int rtotal = rhs.w+rhs.l;
      if (ltotal < rtotal)
         return true;
   }
   return false;
}

bool Record::operator >(const Record &rhs)
{
   float ls = computeScore(w,l);
   float rs = computeScore(rhs.w,rhs.l);
   if (ls > rs)
      return true;
   else if (ls == rs)
   {
      long unsigned int ltotal = w+l;
      long unsigned int rtotal = rhs.w+rhs.l;
      if (ltotal > rtotal)
         return true;
   }
   return false;
}

Database::Database() :
   fd(0),
   drawFd(0),
   actions(NULL),
   draws(NULL)
{
   // Intentionally left blank
}

Database::~Database()
{
   if (fd != 0)
      close(fd);
   if (drawFd != 0)
      close(drawFd);
   if (actions != NULL)
   {
      munmap(actions,sizeof(Record)*Indexer::getNumActions());
   }
   if (draws != NULL)
   {
      munmap(draws,sizeof(Record)*Indexer::getNumDraws());
   }
}

Database& Database::instance()
{
   static Database db;
   return db;
}

bool Database::loadDatabase(char *filename)
{
   fd = open(filename,O_RDWR | O_CREAT,S_IREAD|S_IWRITE|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
   if (fd == -1)
   {
      printf("Failed to open file\n");
      return false;
   }

   // Stretch file size
   if (lseek(fd,sizeof(Record)*Indexer::getNumActions(),SEEK_SET) == -1)
   {
      printf("lseek failed\n");
      return false;
   }

   // Make sure file size increases by writing data to it
   if (write(fd,"",1) == -1)
   {
      printf("write failed\n");
      return false;
   }

   // Map the database
   actions = (Record*) mmap(NULL,sizeof(Record)*Indexer::getNumActions(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
   if (actions == MAP_FAILED)
   {
      printf("Mapping failed!\n");
      return false;
   }

   return true;
}

void Database::recordAction(bool wl, int round, Action action)
{
   if (actions == NULL && !loadDatabase("action_database.db"))
      throw std::runtime_error("Do not have a database loaded.");

   ActionTypeType type = action.type;
   CardIdType cardId = CardId::Invalid;
   if (action.card != NULL)
      cardId = action.card->id;
   int moneyCost = action.cost[Resource::Money];
   int actionCost = action.cost[Resource::Action];
   actionCost += action.cost[Resource::EventAction];
   actionCost += action.cost[Resource::FarmUpkeepAction];
   actionCost += action.cost[Resource::NonFarmUpkeepAction];
   actionCost += action.cost[Resource::BuySellFarmAction];
   actionCost += action.cost[Resource::BuySellNonFarmAction];
   actionCost += action.cost[Resource::SellNonFarmAction];
   int horseCost = action.cost[Resource::Horse];
   int dogCost = action.cost[Resource::Dog];
   int chefCost = action.cost[Resource::Chef];
   int guestCost = action.cost[Resource::Guest];

   size_t ndx = Indexer::getActionNdx(round,type,cardId,moneyCost,actionCost,horseCost,dogCost,chefCost,guestCost);
   if (wl)
      actions[ndx].w++;
   else
      actions[ndx].l++;
}

Record* Database::getActions()
{
   if (actions == NULL && !loadDatabase("action_database.db"))
      throw std::runtime_error("Do not have a database loaded.");

   return actions;
}

float Database::computeScore(Record &record)
{
   return ::computeScore(record.w,record.l);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool Database::loadDrawDB(char *filename)
{
   drawFd = open(filename,O_RDWR | O_CREAT,S_IREAD|S_IWRITE|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
   if (drawFd == -1)
   {
      printf("Failed to open file\n");
      return false;
   }

   // Stretch file size
   if (lseek(drawFd,sizeof(Record)*Indexer::getNumDraws(),SEEK_SET) == -1)
   {
      printf("lseek failed\n");
      return false;
   }

   // Make sure file size increases by writing data to it
   if (write(drawFd,"",1) == -1)
   {
      printf("write failed\n");
      return false;
   }

   // Map the database
   draws = (Record*) mmap(NULL,sizeof(Record)*Indexer::getNumDraws(),PROT_READ|PROT_WRITE,MAP_SHARED,drawFd,0);
   if (draws == MAP_FAILED)
   {
      printf("Mapping failed!\n");
      return false;
   }

   return true;
}

void Database::recordDraw(
   bool wl,
   int round,
   unsigned int properties,
   unsigned int events,
   unsigned int helpers,
   unsigned int companions)
{
   unsigned int ndx = Indexer::getDrawNdx(round,properties,events,helpers,companions);
   if (wl)
      draws[ndx].w++;
   else
      draws[ndx].l++;
}

Record* Database::getDraws()
{
   if (draws == NULL && !loadDrawDB("draws_database.db"))
      throw std::runtime_error("Do not have a draw database loaded.");

   return draws;
}
