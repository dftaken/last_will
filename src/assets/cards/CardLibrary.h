/*
 * CardLibrary.h
 *
 *  Created on: Sep 5, 2018
 *      Author: milites
 */

#ifndef CARDLIBRARY_H_
#define CARDLIBRARY_H_

#include <map>
#include <assets/cards/Card.h>
#include <assets/cards/CardAttributes.h>

class CardLibrary
{
public:
   typedef CardLibrary* Ptr;
   typedef std::map<CardIdType,Card::Ptr> Database;
   typedef std::pair<CardIdType,Card::Ptr> Entry;

   ~CardLibrary();

   static Ptr getInstance();

   Card::Ptr getCard(CardIdType);

   const Database getDatabase();

private:
   CardLibrary();

   static Ptr _instance;

   Database _database;

   void addCompanion(CardIdType,CardNameType,std::string);
   void addProperty(CardIdType,CardNameType,std::string);
   void addEvent(CardIdType,CardNameType,std::string);
   void addHelperExpense(CardIdType,CardNameType,std::string);
   void addSpecial(CardIdType,CardNameType,std::string);
   void addWildCards();
   void addLastWills();
};

#endif /* CARDLIBRARY_H_ */
