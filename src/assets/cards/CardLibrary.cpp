/*
 * CardLibrary.cpp
 *
 *  Created on: Sep 5, 2018
 *      Author: milites
 */

#include <assets/cards/CardLibrary.h>

#include <cstddef>

#include <Logging.h>

using namespace std;

CardDeckTypes CardDeck::Drawable;
CardLibrary::Ptr CardLibrary::_instance = CardLibrary::getInstance();

CardLibrary::CardLibrary() :
   _database()
{
   CardDeck::Drawable.push_back(CardDeck::Events);
   CardDeck::Drawable.push_back(CardDeck::Properties);
   CardDeck::Drawable.push_back(CardDeck::HelpersExpenses);
   CardDeck::Drawable.push_back(CardDeck::Companions);

   addCompanion(CardId::Horse_1,CardName::Horse,"Horse");
   addCompanion(CardId::Horse_2,CardName::Horse,"Horse");
   addCompanion(CardId::Horse_3,CardName::Horse,"Horse");
   addCompanion(CardId::Horse_4,CardName::Horse,"Horse");
   addCompanion(CardId::Horse_5,CardName::Horse,"Horse");
   addCompanion(CardId::Dog_1,CardName::Dog,"Dog");
   addCompanion(CardId::Dog_2,CardName::Dog,"Dog");
   addCompanion(CardId::Dog_3,CardName::Dog,"Dog");
   addCompanion(CardId::Dog_4,CardName::Dog,"Dog");
   addCompanion(CardId::Dog_5,CardName::Dog,"Dog");
   addCompanion(CardId::Chef_1,CardName::Chef,"Chef");
   addCompanion(CardId::Chef_2,CardName::Chef,"Chef");
   addCompanion(CardId::Chef_3,CardName::Chef,"Chef");
   addCompanion(CardId::Chef_4,CardName::Chef,"Chef");
   addCompanion(CardId::Chef_5,CardName::Chef,"Chef");
   addCompanion(CardId::Guest_1,CardName::Guest,"Guest");
   addCompanion(CardId::Guest_2,CardName::Guest,"Guest");
   addCompanion(CardId::Guest_3,CardName::Guest,"Guest");
   addCompanion(CardId::Guest_4,CardName::Guest,"Guest");
   addCompanion(CardId::Guest_5,CardName::Guest,"Guest");
   addProperty(CardId::Farm_1,CardName::Farm,"Farm");
   addProperty(CardId::Farm_2,CardName::Farm,"Farm");
   addProperty(CardId::Farm_3,CardName::Farm,"Farm");
   addProperty(CardId::Farm_4,CardName::Farm,"Farm");
   addProperty(CardId::Farm_5,CardName::Farm,"Farm");
   addProperty(CardId::Farm_6,CardName::Farm,"Farm");
   addProperty(CardId::Farm_7,CardName::Farm,"Farm");
   addProperty(CardId::Farm_8,CardName::Farm,"Farm");
   addProperty(CardId::Farm_9,CardName::Farm,"Farm");
   addProperty(CardId::Farm_10,CardName::Farm,"Farm");
   addProperty(CardId::Farm_11,CardName::Farm,"Farm");
   addProperty(CardId::TownHouse_1,CardName::TownHouse,"TownHouse");
   addProperty(CardId::TownHouse_2,CardName::Farm,"Farm");
   addProperty(CardId::TownHouse_3,CardName::Farm,"Farm");
   addProperty(CardId::TownHouse_4,CardName::Farm,"Farm");
   addProperty(CardId::ManorHouse_1,CardName::ManorHouse,"ManorHouse");
   addProperty(CardId::ManorHouse_2,CardName::ManorHouse,"ManorHouse");
   addProperty(CardId::ManorHouse_3,CardName::ManorHouse,"ManorHouse");
   addProperty(CardId::ManorHouse_4,CardName::ManorHouse,"ManorHouse");
   addProperty(CardId::Mansion_1,CardName::Mansion,"Mansion");
   addProperty(CardId::Mansion_2,CardName::Mansion,"Mansion");
   addProperty(CardId::Mansion_3,CardName::Mansion,"Mansion");
   addProperty(CardId::Mansion_4,CardName::Mansion,"Mansion");
   addProperty(CardId::Mansion_5,CardName::Mansion,"Mansion");
   addEvent(CardId::CarriageRide_1,CardName::CarriageRide,"CarriageRide");
   addEvent(CardId::CarriageRide_2,CardName::CarriageRide,"CarriageRide");
   addEvent(CardId::CarriageRide_3,CardName::CarriageRide,"CarriageRide");
   addEvent(CardId::CarriageRide_4,CardName::CarriageRide,"CarriageRide");
   addEvent(CardId::CarriageRide_5,CardName::CarriageRide,"CarriageRide");
   addEvent(CardId::Theatre_1,CardName::Theatre,"Theatre");
   addEvent(CardId::Theatre_2,CardName::Theatre,"Theatre");
   addEvent(CardId::Theatre_3,CardName::Theatre,"Theatre");
   addEvent(CardId::Theatre_4,CardName::Theatre,"Theatre");
   addEvent(CardId::BoatTrip_1,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_2,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_3,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_4,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_5,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_6,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::BoatTrip_7,CardName::BoatTrip,"BoatTrip");
   addEvent(CardId::Soiree_1,CardName::Soiree,"Soiree");
   addEvent(CardId::Soiree_2,CardName::Soiree,"Soiree");
   addEvent(CardId::Ball_1,CardName::Ball,"Ball");
   addEvent(CardId::Ball_2,CardName::Ball,"Ball");
   addEvent(CardId::Ball_3,CardName::Ball,"Ball");
   addEvent(CardId::Dinner_1,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_2,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_3,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_4,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_5,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_6,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_7,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_8,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_9,CardName::Dinner,"Dinner");
   addEvent(CardId::Dinner_10,CardName::Dinner,"Dinner");
   addHelperExpense(CardId::GentlemensClub_1,CardName::GentlemensClub,"GentlemensClub");
   addHelperExpense(CardId::GentlemensClub_2,CardName::GentlemensClub,"GentlemensClub");
   addHelperExpense(CardId::Reservation_1,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Reservation_2,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Reservation_3,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Reservation_4,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Reservation_5,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Reservation_6,CardName::Reservation,"Reservation");
   addHelperExpense(CardId::Carriage_1,CardName::Carriage,"Carriage");
   addHelperExpense(CardId::Carriage_2,CardName::Carriage,"Carriage");
   addHelperExpense(CardId::Carriage_3,CardName::Carriage,"Carriage");
   addHelperExpense(CardId::Carriage_4,CardName::Carriage,"Carriage");
   addHelperExpense(CardId::Carriage_5,CardName::Carriage,"Carriage");
   addHelperExpense(CardId::TrainingGround_1,CardName::TrainingGround,"TrainingGround");
   addHelperExpense(CardId::TrainingGround_2,CardName::TrainingGround,"TrainingGround");
   addHelperExpense(CardId::TrainingGround_3,CardName::TrainingGround,"TrainingGround");
   addHelperExpense(CardId::TrainingGround_4,CardName::TrainingGround,"TrainingGround");
   addHelperExpense(CardId::TrainingGround_5,CardName::TrainingGround,"TrainingGround");
   addHelperExpense(CardId::Coachman_1,CardName::Coachman,"CoachMan");
   addHelperExpense(CardId::Coachman_2,CardName::Coachman,"CoachMan");
   addHelperExpense(CardId::EstateAgent_1,CardName::EstateAgent,"EstateAgent");
   addHelperExpense(CardId::EstateAgent_2,CardName::EstateAgent,"EstateAgent");
   addHelperExpense(CardId::EstateAgent_3,CardName::EstateAgent,"EstateAgent");
   addHelperExpense(CardId::Gardener_1,CardName::Gardener,"Gardener");
   addHelperExpense(CardId::Gardener_2,CardName::Gardener,"Gardener");
   addHelperExpense(CardId::Gardener_3,CardName::Gardener,"Gardener");
   addHelperExpense(CardId::SchoolChum_1,CardName::SchoolChum,"SchoolChum");
   addHelperExpense(CardId::SchoolChum_2,CardName::SchoolChum,"SchoolChum");
   addHelperExpense(CardId::SchoolChum_3,CardName::SchoolChum,"SchoolChum");
   addHelperExpense(CardId::SchoolChum_4,CardName::SchoolChum,"SchoolChum");
   addHelperExpense(CardId::SeaDog,CardName::SeaDog,"SeaDog");
   addHelperExpense(CardId::Steward_1,CardName::Steward,"Steward");
   addHelperExpense(CardId::Steward_2,CardName::Steward,"Steward");
   addHelperExpense(CardId::Steward_3,CardName::Steward,"Steward");
   addHelperExpense(CardId::Tailor,CardName::Tailor,"Tailor");
   addHelperExpense(CardId::Valet,CardName::Valet,"Valet");
   addHelperExpense(CardId::Waiter_1,CardName::Waiter,"Waiter");
   addHelperExpense(CardId::Waiter_2,CardName::Waiter,"Waiter");
   addHelperExpense(CardId::Waiter_3,CardName::Waiter,"Waiter");
   addSpecial(CardId::BreedersFair,CardName::BreedersFair,"BreedersFair");
   addSpecial(CardId::WildParty,CardName::WildParty,"WildParty");
   addSpecial(CardId::HecticDay_1,CardName::HecticDay,"HecticDay");
   addSpecial(CardId::HecticDay_2,CardName::HecticDay,"HecticDay");
   addSpecial(CardId::HecticDay_3,CardName::HecticDay,"HecticDay");
   addSpecial(CardId::Ball_S,CardName::Ball,"Ball");
   addSpecial(CardId::CarriageRide_S,CardName::CarriageRide,"CarriageRide");
   addSpecial(CardId::Dinner_S,CardName::Dinner,"Dinner");
   addSpecial(CardId::OldFriend_1,CardName::OldFriend,"OldFriend");
   addSpecial(CardId::OldFriend_2,CardName::OldFriend,"OldFriend");
   addSpecial(CardId::OldFriend_3,CardName::OldFriend,"OldFriend");
   addSpecial(CardId::OldFriend_4,CardName::OldFriend,"OldFriend");
   addSpecial(CardId::SchoolChum_S,CardName::SchoolChum,"SchoolChum");
   addSpecial(CardId::EstateAgent_S1,CardName::EstateAgent,"EstateAgent");
   addSpecial(CardId::EstateAgent_S2,CardName::EstateAgent,"EstateAgent");
   addSpecial(CardId::Steward_S1,CardName::Steward,"Steward");
   addSpecial(CardId::Steward_S2,CardName::Steward,"Steward");
   addWildCards();
   addLastWills();
   printf("# of cards loaded in library = %lu\n",_database.size());
   size_t maxNum = 0;
   for (Database::iterator itr = _database.begin(); itr != _database.end(); ++itr)
   {
      if (itr->second->group == CardGroup::Event &&
          itr->second->valueLevels.size() == 1 &&
          itr->second->upgradeLevels.size() > maxNum)
         maxNum = itr->second->upgradeLevels.size();
   }
   printf("Max num upgrade levels for events = %lu\n",maxNum);
}

CardLibrary::~CardLibrary()
{
   // Delete card instances
   for (Database::iterator itr = _database.begin(); itr != _database.end(); ++itr)
   {
      delete itr->second;
   }
}

CardLibrary::Ptr CardLibrary::getInstance()
{
   if (_instance == NULL)
   {
      _instance = new CardLibrary();
   }
   return _instance;
}

Card::Ptr CardLibrary::getCard(CardIdType id)
{
   Card::Ptr ptr = NULL;
   Database::iterator itr = _database.find(id);
   if (itr != _database.end())
   {
      ptr = itr->second;
   }
   return ptr;
}

const CardLibrary::Database CardLibrary::getDatabase()
{
   return _database;
}

void CardLibrary::addCompanion(
   CardIdType id,
   CardNameType name,
   string strName)
{
   Card::Ptr card = new Card();
   card->id = id;
   card->name = name;
   card->strName = strName;
   card->group = CardGroup::Companion;
   card->deck = CardDeck::Companions;
   switch (name)
   {
      case CardName::Horse:
         card->type = CardType::Horse;
         break;
      case CardName::Dog:
         card->type = CardType::Dog;
         break;
      case CardName::Chef:
         card->type = CardType::Chef;
         break;
      case CardName::Guest:
         card->type = CardType::Guest;
         break;
      default:
         break;
   }
   _database.insert(Entry(id,card));
}

void CardLibrary::addProperty(
   CardIdType id,
   CardNameType name,
   string strName)
{
   Card::Ptr card = new Card();
   card->id = id;
   card->name = name;
   card->strName = strName;
   card->group = CardGroup::Property;
   card->deck = CardDeck::Properties;
   card->expenseCost = 1;
   switch (name)
   {
      case CardName::Farm:
         card->type = CardType::Farm;
         break;
      case CardName::TownHouse:
         card->type = CardType::TownHouse;
         break;
      case CardName::ManorHouse:
         card->type = CardType::ManorHouse;
         break;
      case CardName::Mansion:
         card->type = CardType::Mansion;
         break;
      default:
         break;
   }
   switch (id)
   {
      case CardId::Farm_1:
         card->addValueLvl(Resource::Money,16);
         card->addUpgradeLvl(Resource::Base,3);
         card->addUpgradeLvl(Resource::Horse,5);
         card->addUpgradeLvl(Resource::Horse,8);
         break;
      case CardId::Farm_2:
         card->addValueLvl(Resource::Money,12);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,4);
         card->addUpgradeLvl(Resource::Horse,7);
         break;
      case CardId::Farm_3:
         card->addValueLvl(Resource::Money,8);
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Dog,3);
         break;
      case CardId::Farm_4:
         card->addValueLvl(Resource::Money,10);
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Horse,3);
         card->addUpgradeLvl(Resource::Dog,7);
         break;
      case CardId::Farm_5:
         card->addValueLvl(Resource::Money,25);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Horse,7);
         card->addUpgradeLvl(Resource::Horse,10);
         break;
      case CardId::Farm_6:
         card->addValueLvl(Resource::Money,20);
         card->addUpgradeLvl(Resource::Base,3);
         card->addUpgradeLvl(Resource::Horse,7);
         break;
      case CardId::Farm_7:
         card->addValueLvl(Resource::Money,12);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,3);
         card->addUpgradeLvl(Resource::Dog,6);
         break;
      case CardId::Farm_8:
         card->addValueLvl(Resource::Money,9);
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Dog,4);
         break;
      case CardId::Farm_9:
         card->addValueLvl(Resource::Money,14);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,3);
         card->addUpgradeLvl(Resource::Dog,5);
         card->addUpgradeLvl(Resource::Dog,9);
         break;
      case CardId::Farm_10:
         card->addValueLvl(Resource::Money,15);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Horse,6);
         break;
      case CardId::Farm_11:
         card->addValueLvl(Resource::Money,13);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Horse,4);
         card->addUpgradeLvl(Resource::Horse,6);
         break;
      case CardId::TownHouse_1:
         card->addValueLvl(Resource::Money,15);
         card->addValueLvl(Resource::Money,13);
         card->addValueLvl(Resource::Money,10);
         card->addValueLvl(Resource::Money,7);
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::TownHouse_2:
         card->addValueLvl(Resource::Money,17);
         card->addValueLvl(Resource::Money,15);
         card->addValueLvl(Resource::Money,13);
         card->addValueLvl(Resource::Money,11);
         card->addValueLvl(Resource::Money,8);
         card->addValueLvl(Resource::Money,5);
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::TownHouse_3:
         card->addValueLvl(Resource::Money,13);
         card->addValueLvl(Resource::Money,12);
         card->addValueLvl(Resource::Money,11);
         card->addUpgradeLvl(Resource::Base,4);
         break;
      case CardId::TownHouse_4:
         card->addValueLvl(Resource::Money,15);
         card->addValueLvl(Resource::Money,13);
         card->addUpgradeLvl(Resource::Base,3);
         card->addUpgradeLvl(Resource::Chef,5);
         card->addUpgradeLvl(Resource::Guest,8);
         break;
      case CardId::ManorHouse_1:
         card->addValueLvl(Resource::Money,23);
         card->addValueLvl(Resource::Money,22);
         card->addValueLvl(Resource::Money,20);
         card->addUpgradeLvl(Resource::Base,5);
         card->addUpgradeLvl(Resource::Chef,7);
         card->addUpgradeLvl(Resource::Guest,10);
         break;
      case CardId::ManorHouse_2:
         card->addValueLvl(Resource::Money,18);
         card->addValueLvl(Resource::Money,15);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Chef,6);
         break;
      case CardId::ManorHouse_3:
         card->addValueLvl(Resource::Money,20);
         card->addValueLvl(Resource::Money,19);
         card->addValueLvl(Resource::Money,17);
         card->addValueLvl(Resource::Money,13);
         card->addUpgradeLvl(Resource::Base,4);
         break;
      case CardId::ManorHouse_4:
         card->addValueLvl(Resource::Money,10);
         card->addValueLvl(Resource::Money,8);
         card->addValueLvl(Resource::Money,7);
         card->addValueLvl(Resource::Money,5);
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::Mansion_1:
         card->addValueLvl(Resource::Money,13);
         card->addValueLvl(Resource::Money,11);
         card->addValueLvl(Resource::Money,9);
         card->addValueLvl(Resource::Money,7);
         card->addValueLvl(Resource::Money,5);
         card->addValueLvl(Resource::Money,3);
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::Mansion_2:
         card->addValueLvl(Resource::Money,8);
         card->addValueLvl(Resource::Money,6);
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Guest,5);
         break;
      case CardId::Mansion_3:
         card->addValueLvl(Resource::Money,15);
         card->addValueLvl(Resource::Money,13);
         card->addValueLvl(Resource::Money,11);
         card->addValueLvl(Resource::Money,9);
         card->addValueLvl(Resource::Money,4);
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::Mansion_4:
         card->addValueLvl(Resource::Money,12);
         card->addValueLvl(Resource::Money,10);
         card->addValueLvl(Resource::Money,8);
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Chef,5);
         break;
      case CardId::Mansion_5:
         card->addValueLvl(Resource::Money,10);
         card->addValueLvl(Resource::Money,9);
         card->addValueLvl(Resource::Money,7);
         card->addValueLvl(Resource::Money,5);
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Guest,5);
         break;
      default:
         break;
   }
   _database.insert(Entry(id,card));
}

void CardLibrary::addEvent(
   CardIdType id,
   CardNameType name,
   string strName)
{
   Card::Ptr card = new Card();
   card->id = id;
   card->name = name;
   card->strName = strName;
   card->group = CardGroup::Event;
   card->deck = CardDeck::Events;
   card->additiveUpgrades = true;
   card->addValueLvl(Resource::Action,1);
   switch (name)
   {
      case CardName::CarriageRide:
         card->type = CardType::Carriage;
         break;
      case CardName::Theatre:
         card->type = CardType::Theatre;
         break;
      case CardName::BoatTrip:
         card->type = CardType::BoatTrip;
         break;
      case CardName::Soiree:
         card->type = CardType::Party;
         break;
      case CardName::Ball:
         card->type = CardType::Party;
         break;
      case CardName::Dinner:
         card->type = CardType::Dinner;
         break;
      default:
         break;
   }
   switch (id)
   {
      case CardId::CarriageRide_1:
      case CardId::CarriageRide_2:
         card->addUpgradeLvl(Resource::Base,2);
         break;
      case CardId::CarriageRide_3:
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::CarriageRide_4:
      case CardId::CarriageRide_5:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Guest,2);
         break;
      case CardId::Theatre_1:
      case CardId::Theatre_2:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Horse,3);
         break;
      case CardId::Theatre_3:
      case CardId::Theatre_4:
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::BoatTrip_1:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Guest,2);
         card->addUpgradeLvl(Resource::Dog,3);
         card->addUpgradeLvl(Resource::Chef,2);
         break;
      case CardId::BoatTrip_2:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Guest,2);
         card->addUpgradeLvl(Resource::Chef,2);
         break;
      case CardId::BoatTrip_3:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Chef,3);
         break;
      case CardId::BoatTrip_4:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,5);
         break;
      case CardId::BoatTrip_5:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Guest,3);
         card->addUpgradeLvl(Resource::Dog,2);
         card->addUpgradeLvl(Resource::Chef,2);
         break;
      case CardId::BoatTrip_6:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,3);
         card->addUpgradeLvl(Resource::Guest,2);
         card->addUpgradeLvl(Resource::Dog,2);
         card->addUpgradeLvl(Resource::Chef,2);
         break;
      case CardId::BoatTrip_7:
         card->valueLevels[0].value = 2;
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Guest,3);
         break;
      case CardId::Soiree_1:
         card->addUpgradeLvl(Resource::Base,2);
         break;
      case CardId::Soiree_2:
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::Ball_1:
         card->valueLevels[0].value = 2;
         card->addValueLvl(Resource::Action,3);
         card->addValueLvl(Resource::Action,4);
         card->addValueLvl(Resource::Action,5);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Action,6);
         card->addUpgradeLvl(Resource::Action,10);
         card->addUpgradeLvl(Resource::Action,13);
         card->additiveUpgrades = false;
         break;
      case CardId::Ball_2:
         card->valueLevels[0].value = 2;
         card->addValueLvl(Resource::Action,3);
         card->addValueLvl(Resource::Action,4);
         card->addValueLvl(Resource::Action,5);
         card->addValueLvl(Resource::Action,6);
         card->addValueLvl(Resource::Action,7);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Action,6);
         card->addUpgradeLvl(Resource::Action,8);
         card->addUpgradeLvl(Resource::Action,12);
         card->addUpgradeLvl(Resource::Action,15);
         card->addUpgradeLvl(Resource::Action,22);
         card->additiveUpgrades = false;
         break;
      case CardId::Ball_3:
         card->valueLevels[0].value = 2;
         card->addValueLvl(Resource::Action,3);
         card->addValueLvl(Resource::Action,4);
         card->addValueLvl(Resource::Action,5);
         card->addValueLvl(Resource::Action,6);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Action,6);
         card->addUpgradeLvl(Resource::Action,9);
         card->addUpgradeLvl(Resource::Action,12);
         card->addUpgradeLvl(Resource::Action,17);
         card->additiveUpgrades = false;
         break;
      case CardId::Dinner_1:
      case CardId::Dinner_2:
      case CardId::Dinner_3:
         card->addUpgradeLvl(Resource::Base,2);
         break;
      case CardId::Dinner_4:
      case CardId::Dinner_5:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Horse,2);
         break;
      case CardId::Dinner_6:
      case CardId::Dinner_7:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,2);
         break;
      case CardId::Dinner_8:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Guest,2);
         card->addUpgradeLvl(Resource::Guest,2);
         break;
      case CardId::Dinner_9:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,2);
         break;
      case CardId::Dinner_10:
         card->addUpgradeLvl(Resource::Base,2);
         card->addUpgradeLvl(Resource::Dog,2);
         break;
      default:
         break;
   }
   _database.insert(Entry(id,card));
}

void CardLibrary::addHelperExpense(
   CardIdType id,
   CardNameType name,
   string strName)
{
   Card::Ptr card = new Card();
   card->id = id;
   card->name = name;
   card->strName = strName;
   card->group = CardGroup::HelperExpense;
   card->deck = CardDeck::HelpersExpenses;
   card->addValueLvl(Resource::Action,1);
   switch (name)
   {
      case CardName::GentlemensClub:
         card->type = CardType::Party;
         break;
      case CardName::Reservation:
         card->type = CardType::Dinner;
         break;
      case CardName::Carriage:
         card->type = CardType::Carriage;
         break;
      case CardName::TrainingGround:
         card->type = CardType::TrainingGround;
         break;
      default:
         card->type = CardType::Helper;
         break;
   }
   switch (id)
   {
      case CardId::GentlemensClub_1:
      case CardId::GentlemensClub_2:
         card->valueLevels[0].value = 2;
         card->expenseCost = 1;
         card->addUpgradeLvl(Resource::Base,5);
         break;
      case CardId::Reservation_1:
      case CardId::Reservation_2:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,2);
         break;
      case CardId::Reservation_3:
      case CardId::Reservation_4:
         card->expenseCost = 1;
         card->addUpgradeLvl(Resource::Base,4);
         break;
      case CardId::Reservation_5:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Chef,2);
         card->addUpgradeLvl(Resource::Chef,4);
         break;
      case CardId::Reservation_6:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Chef,3);
         break;
      case CardId::Carriage_1:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         card->addUpgradeLvl(Resource::Guest,3);
         break;
      case CardId::Carriage_2:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         break;
      case CardId::Carriage_3:
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,2);
         break;
      case CardId::Carriage_4:
      case CardId::Carriage_5:
         card->expenseCost = 1;
         card->addUpgradeLvl(Resource::Base,3);
         break;
      case CardId::TrainingGround_1:
         card->effect = Effect::Spend1PerFarmAndDogOnFarm;
         break;
      case CardId::TrainingGround_2:
         card->effect = Effect::Spend2PerHorseOnFarm;
         break;
      case CardId::TrainingGround_3:
         card->effect = Effect::Spend2PerFarm;
         break;
      case CardId::TrainingGround_4:
         card->effect = Effect::Spend1PerFarmAndDogOnFarm;
         break;
      case CardId::TrainingGround_5:
         card->effect = Effect::Spend2PerDogOnFarm;
         break;
      case CardId::Coachman_1:
         card->effect = Effect::CarriageSpend1Extra;
         break;
      case CardId::Coachman_2:
         card->valueLevels[0].value = 2;
         card->effect = Effect::CarriageSpend2Extra;
         break;
      case CardId::EstateAgent_1:
      case CardId::EstateAgent_2:
         card->effect = Effect::AdjustNonFarmValueBy2WhenBuySell;
         break;
      case CardId::EstateAgent_3:
         card->effect = Effect::ReduceFarmValueBy2AndBuySell;
         break;
      case CardId::Gardener_1:
         card->effect = Effect::TownAndManorUpkeep3Extra;
         break;
      case CardId::Gardener_2:
         card->effect = Effect::MansionAndManorUpkeep3Extra;
         break;
      case CardId::Gardener_3:
         card->effect = Effect::MansionAndTownUpkeep3Extra;
         break;
      case CardId::SchoolChum_1:
      case CardId::SchoolChum_2:
         card->effect = Effect::Plus2MaxHandSize;
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         break;
      case CardId::SchoolChum_3:
         card->effect = Effect::Draw3Companions;
         break;
      case CardId::SchoolChum_4:
         card->effect = Effect::Draw3Events;
         break;
      case CardId::SeaDog:
         card->effect = Effect::BoatTripAndPartySpend2Extra;
         break;
      case CardId::Steward_1:
         card->effect = Effect::Perform1NonFarmUpkeep;
         break;
      case CardId::Steward_2:
      case CardId::Steward_3:
         card->effect = Effect::Perform1FarmUpkeep;
         break;
      case CardId::Tailor:
         card->effect = Effect::TheatreSpend3Extra;
         break;
      case CardId::Valet:
         card->effect = Effect::Gain1EventAction;
         break;
      case CardId::Waiter_1:
      case CardId::Waiter_2:
      case CardId::Waiter_3:
         card->effect = Effect::DinnerSpend1Extra;
         break;
      default:
         break;
   }
   _database.insert(Entry(id,card));
}

void CardLibrary::addSpecial(
   CardIdType id,
   CardNameType name,
   string strName)
{
   Card::Ptr card = new Card();
   card->id = id;
   card->name = name;
   card->strName = strName;
   card->deck = CardDeck::Special;
   card->addValueLvl(Resource::Action,1);
   switch (id)
   {
      case CardId::BreedersFair:
         card->group = CardGroup::Event;
         card->type = CardType::None;
         card->crowns = 1;
         card->effect = Effect::PlaceDogOrHorseOnFarmTwice;
         break;
      case CardId::WildParty:
         card->group = CardGroup::Event;
         card->type = CardType::None;
         card->crowns = 2;
         card->effect = Effect::DepreciateAHouseValue3Times;
         break;
      case CardId::HecticDay_1:
         card->group = CardGroup::Event;
         card->type = CardType::None;
         card->crowns = 2;
         card->effect = Effect::Gain3Actions;
         break;
      case CardId::HecticDay_2:
         card->valueLevels[0].resource = Resource::ActionAnd2Money;
         card->group = CardGroup::Event;
         card->type = CardType::None;
         card->crowns = 3;
         card->effect = Effect::Gain2Actions;
         break;
      case CardId::HecticDay_3:
         card->group = CardGroup::Event;
         card->type = CardType::None;
         card->crowns = 3;
         card->effect = Effect::Gain3Actions;
         break;
      case CardId::Ball_S:
         card->group = CardGroup::Event;
         card->type = CardType::Party;
         card->crowns = 2;
         card->valueLevels[0].value = 2;
         card->addValueLvl(Resource::Action,3);
         card->addValueLvl(Resource::Action,4);
         card->addValueLvl(Resource::Action,5);
         card->addValueLvl(Resource::Action,6);
         card->addValueLvl(Resource::Action,7);
         card->addUpgradeLvl(Resource::Base,4);
         card->addUpgradeLvl(Resource::Action,6);
         card->addUpgradeLvl(Resource::Action,10);
         card->addUpgradeLvl(Resource::Action,13);
         card->addUpgradeLvl(Resource::Action,17);
         card->addUpgradeLvl(Resource::Action,25);
         break;
      case CardId::CarriageRide_S:
         card->group = CardGroup::Event;
         card->type = CardType::Carriage;
         card->crowns = 3;
         card->addUpgradeLvl(Resource::Base,4);
         break;
      case CardId::Dinner_S:
         card->group = CardGroup::Event;
         card->type = CardType::Dinner;
         card->crowns = 3;
         card->addUpgradeLvl(Resource::Base,4);
         break;
      case CardId::OldFriend_1:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 3;
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,2);
         card->effect = Effect::Gain1Action;
         break;
      case CardId::OldFriend_2:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 2;
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,1);
         card->effect = Effect::Gain1Action;
         break;
      case CardId::OldFriend_3:
      case CardId::OldFriend_4:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 1;
         card->effect = Effect::Gain1Action;
         break;
      case CardId::SchoolChum_S:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 2;
         card->expenseCost = 0;
         card->addUpgradeLvl(Resource::Base,2);
         card->effect = Effect::Draw2Cards;
         break;
      case CardId::EstateAgent_S1:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 1;
         card->valueLevels[0].value = 2;
         card->effect = Effect::BuySell2NonFarms;
         break;
      case CardId::EstateAgent_S2:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 3;
         card->effect = Effect::ReduceNonFarmBy4WhenBuySell;
         break;
      case CardId::Steward_S1:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 1;
         card->valueLevels[0].value = 2;
         card->effect = Effect::Perform2FarmUpkeep;
         break;
      case CardId::Steward_S2:
         card->group = CardGroup::HelperExpense;
         card->type = CardType::Helper;
         card->crowns = 1;
         card->addUpgradeLvl(Resource::Base,1);
         card->effect = Effect::Perform1NonFarmUpkeep;
         break;
      default:
         break;
   }
   _database.insert(Entry(id,card));
}

void CardLibrary::addWildCards()
{
   Card::Ptr card1 = new Card();
   card1->id = CardId::WildCard_1;
   card1->name = CardName::WildCard;
   card1->strName = "WildCard";
   card1->group = CardGroup::Companion;
   card1->deck = CardDeck::WildCards;
   card1->type = CardType::WildCard;
   _database.insert(Entry(card1->id,card1));

   Card::Ptr card2 = new Card();
   card2->id = CardId::WildCard_2;
   card2->name = CardName::WildCard;
   card2->strName = "WildCard";
   card2->group = CardGroup::Companion;
   card2->deck = CardDeck::WildCards;
   card2->type = CardType::WildCard;
   _database.insert(Entry(card2->id,card2));
}

void CardLibrary::addLastWills()
{
   for (int i = 0; i < 7; ++i)
   {
      Card::Ptr card = new Card();
      card->name = CardName::StartMoney;
      card->strName = "StartMoney";
      card->group = CardGroup::Other;
      card->deck = CardDeck::LastWill;
      switch (i)
      {
         case 0:
            card->id = CardId::StartMoney_1;
            card->addValueLvl(Resource::Money,70);
            break;
         case 1:
            card->id = CardId::StartMoney_2;
            card->addValueLvl(Resource::Money,80);
            break;
         case 2:
            card->id = CardId::StartMoney_3;
            card->addValueLvl(Resource::Money,90);
            break;
         case 3:
            card->id = CardId::StartMoney_4;
            card->addValueLvl(Resource::Money,100);
            break;
         case 4:
            card->id = CardId::StartMoney_5;
            card->addValueLvl(Resource::Money,110);
            break;
         case 5:
            card->id = CardId::StartMoney_6;
            card->addValueLvl(Resource::Money,120);
            break;
         case 6:
            card->id = CardId::StartMoney_7;
            card->addValueLvl(Resource::Money,130);
            break;
         default:
            break;
      }
      _database.insert(Entry(card->id,card));
   }
}
