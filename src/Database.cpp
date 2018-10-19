/*
 * Database.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: milites
 */

#include <Database.h>
#include <Indexer.h>

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

Database::Database() :
   fd(0),
   actions(NULL)
{
   // Intentionally left blank
}

Database::~Database()
{
   if (fd != 0)
      close(fd);
   if (actions != NULL)
   {
      for (size_t i = 0; i < Indexer::getNumActions(); ++i)
      {
//         if (actions[i].w != 0 || actions[i].l != 0)
//            printf("Action %lu: W = %u , L = %u\n",i,actions[i].w,actions[i].l);
      }
      munmap(actions,sizeof(ActionRecord)*Indexer::getNumActions());
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
   if (lseek(fd,sizeof(ActionRecord)*Indexer::getNumActions(),SEEK_SET) == -1)
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
   actions = (ActionRecord*) mmap(NULL,sizeof(ActionRecord)*Indexer::getNumActions(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
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
