/*
 * CardIds.h
 *
 *  Created on: Sep 5, 2018
 *      Author: milites
 */

#ifndef CARDIDS_H_
#define CARDIDS_H_

#include <vector>
#include <map>
#include <string>
#include <Logging.h>

struct CardId
{
   enum Type {
      Invalid=-1,
      Horse_1=0,
      Horse_2,
      Horse_3,
      Horse_4,
      Horse_5,
      Dog_1,
      Dog_2,
      Dog_3,
      Dog_4,
      Dog_5,
      Chef_1,
      Chef_2,
      Chef_3,
      Chef_4,
      Chef_5,
      Guest_1,
      Guest_2,
      Guest_3,
      Guest_4,
      Guest_5,
      Farm_1,
      Farm_2,
      Farm_3,
      Farm_4,
      Farm_5,
      Farm_6,
      Farm_7,
      Farm_8,
      Farm_9,
      Farm_10,
      Farm_11,
      TownHouse_1,
      TownHouse_2,
      TownHouse_3,
      TownHouse_4,
      ManorHouse_1,
      ManorHouse_2,
      ManorHouse_3,
      ManorHouse_4,
      Mansion_1,
      Mansion_2,
      Mansion_3,
      Mansion_4,
      Mansion_5,
      CarriageRide_1,
      CarriageRide_2,
      CarriageRide_3,
      CarriageRide_4,
      CarriageRide_5,
      Theatre_1,
      Theatre_2,
      Theatre_3,
      Theatre_4,
      BoatTrip_1,
      BoatTrip_2,
      BoatTrip_3,
      BoatTrip_4,
      BoatTrip_5,
      BoatTrip_6,
      BoatTrip_7,
      Soiree_1,
      Soiree_2,
      Ball_1,
      Ball_2,
      Ball_3,
      Dinner_1,
      Dinner_2,
      Dinner_3,
      Dinner_4,
      Dinner_5,
      Dinner_6,
      Dinner_7,
      Dinner_8,
      Dinner_9,
      Dinner_10,
      GentlemensClub_1,
      GentlemensClub_2,
      Reservation_1,
      Reservation_2,
      Reservation_3,
      Reservation_4,
      Reservation_5,
      Reservation_6,
      Carriage_1,
      Carriage_2,
      Carriage_3,
      Carriage_4,
      Carriage_5,
      TrainingGround_1,
      TrainingGround_2,
      TrainingGround_3,
      TrainingGround_4,
      TrainingGround_5,
      SeaDog,
      Gardener_1,
      Gardener_2,
      Gardener_3,
      Valet,
      EstateAgent_1,
      EstateAgent_2,
      EstateAgent_3,
      Waiter_1,
      Waiter_2,
      Waiter_3,
      SchoolChum_1,
      SchoolChum_2,
      SchoolChum_3,
      SchoolChum_4,
      Steward_1,
      Steward_2,
      Steward_3,
      Tailor,
      Coachman_1,
      Coachman_2,
      BreedersFair,
      WildParty,
      HecticDay_1,
      HecticDay_2,
      HecticDay_3,
      Ball_S,
      CarriageRide_S,
      Dinner_S,
      OldFriend_1,
      OldFriend_2,
      OldFriend_3,
      OldFriend_4,
      SchoolChum_S,
      EstateAgent_S1,
      EstateAgent_S2,
      Steward_S1,
      Steward_S2,
      WildCard_1,
      WildCard_2,
      StartMoney_1,
      StartMoney_2,
      StartMoney_3,
      StartMoney_4,
      StartMoney_5,
      StartMoney_6,
      StartMoney_7,
      NumCards
   };
   static std::string toString(Type);
};
typedef CardId::Type CardIdType;

// CardDeck represents the deck in which the card lives
struct CardDeck
{
   enum Type {
      Invalid=-1,
      Events=0,
      Properties,
      HelpersExpenses,
      Companions,
      Special,
      WildCards,
      LastWill,
      NumDeckTypes
   };
   static std::vector<Type> Drawable;
};
typedef CardDeck::Type CardDeckType;
typedef std::vector<CardDeckType> CardDeckTypes;

// CardGroup represents the general function of the card
struct CardGroup
{
   enum Type {
      Invalid=-1,
      Event=0,
      Property,
      HelperExpense,
      Companion,
      Other
   };
};
typedef CardGroup::Type CardGroupType;

// CardType represents the general
struct CardType
{
   enum Type {
      Invalid=-1,
      None=0,
      Horse,
      Dog,
      Chef,
      Guest,
      Farm,
      TownHouse,
      ManorHouse,
      Mansion,
      Carriage,
      Theatre,
      BoatTrip,
      Party,
      Dinner,
      Helper,
      TrainingGround,
      WildCard,
      LastWill
   };
};
typedef CardType::Type CardTypeType;

// CardName can cross decks/classes
struct CardName
{
   enum Type {
      Invalid=-1,
      Horse=0,
      Dog,
      Chef,
      Guest,
      Farm,
      TownHouse,
      ManorHouse,
      Mansion,
      CarriageRide,
      Theatre,
      BoatTrip,
      Soiree,
      Ball,
      Dinner,
      GentlemensClub,
      Reservation,
      Carriage,
      TrainingGround,
      SeaDog,
      Gardener,
      Valet,
      EstateAgent,
      Waiter,
      SchoolChum,
      Steward,
      Tailor,
      Coachman,
      BreedersFair,
      WildParty,
      HecticDay,
      OldFriend,
      WildCard,
      StartMoney
   };
};
typedef CardName::Type CardNameType;

struct Resource
{
   enum Type {
      Invalid=-1,
      Base=0,
      Money,
      BoardSpot,
      Action,
      EventAction,
      FarmUpkeepAction,
      NonFarmUpkeepAction,
      BuySellFarmAction,
      BuySellNonFarmAction,
      SellNonFarmAction,
      Horse,
      Dog,
      Chef,
      Guest,
      WildCard,
      ActionAnd2Money
   };
   static std::string toString(Type);
};
typedef Resource::Type ResourceType;
struct Resources : public std::map<ResourceType,int>
{
   bool operator==(const Resources &rhs)
   {
      bool result = false;
      if (rhs.size() == size())
      {
         bool allResourcesMatched = true;
         for (std::map<ResourceType,int>::iterator itr = begin(); itr != end(); ++itr)
         {
            std::map<ResourceType,int>::const_iterator ritr = rhs.find(itr->first);
            if (!(ritr != rhs.end() && ritr->second == itr->second))
            {
               allResourcesMatched = false;
               break;
            }
         }

         if (allResourcesMatched)
         {
            result = true;
         }
      }
      return result;
   }

   void print()
   {
      printf("Resources:\n");
      for (std::map<ResourceType,int>::iterator itr = begin(); itr != end(); ++itr)
      {
         printf("   %s = %d\n",Resource::toString(itr->first).c_str(),itr->second);
      }
   }
};
//typedef std::map<ResourceType,int> Resources;

struct Effect
{
   enum Type {
      None=-1,
      Spend1PerFarmAndHorseOnFarm=0,
      Spend2PerHorseOnFarm,
      Spend2PerFarm,
      Spend1PerFarmAndDogOnFarm,
      Spend2PerDogOnFarm,
      BoatTripAndPartySpend2Extra,
      TownAndManorUpkeep3Extra,
      MansionAndManorUpkeep3Extra,
      Gain1EventAction,
      AdjustNonFarmValueBy2WhenBuySell,
      DinnerSpend1Extra,
      Plus2MaxHandSize,
      MansionAndTownUpkeep3Extra,
      Perform1NonFarmUpkeep,
      Perform1FarmUpkeep,
      ReduceFarmValueBy2AndBuySell,
      Draw3Companions,
      CarriageSpend1Extra,
      CarriageSpend2Extra,
      TheatreSpend3Extra,
      Draw3Events,
      PlaceDogOrHorseOnFarmTwice,
      DepreciateAHouseValue3Times,
      Gain1Action,
      Gain2Actions,
      Gain3Actions,
      Draw2Cards,
      BuySell2NonFarms,
      Perform2FarmUpkeep,
      ReduceNonFarmBy4WhenBuySell
   };

   static std::string toString(Type);

   // Placing involves buying/selling property, putting a card
   // on the player board.
   // Expense involves using a card (activating a card on the
   // player board or an event from the hand) for effect of
   // spending money.
   static std::vector<Type> getEffectsRelatedToPlacing(CardTypeType);
   static std::vector<Type> getEffectsRelatedToExpense(CardTypeType);
   static std::vector<Type> getEffectsWithLimitedActivation();
};
typedef Effect::Type EffectType;
typedef std::vector<EffectType> Effects;

/*
 * UpgradeLevel represents the expense/upkeep element of a card
 */
struct UpgradeLevel
{
   ResourceType cost; // Companion Cost
   int expense;

   UpgradeLevel(ResourceType inCost, int inExpense) :
      cost(inCost),
      expense(inExpense)
   {};

   UpgradeLevel() :
      cost(Resource::Invalid),
      expense(0)
   {};
};
typedef std::vector<UpgradeLevel> UpgradeLevels;

/*
 * ValueLevel represents the cost to play/bring out a card
 */
struct ValueLevel
{
   // Resource required to unlock this value level
   ResourceType resource;

   // Value is ALWAYS in terms of money
   int value;

   ValueLevel(ResourceType inResource, int inValue) :
      resource(inResource),
      value(inValue)
   {};

   ValueLevel(int inValue) :
      resource(Resource::Action),
      value(inValue)
   {};

   ValueLevel() :
      resource(Resource::Invalid),
      value(0)
   {};
};
typedef std::vector<ValueLevel> ValueLevels;

#endif /* CARDIDS_H_ */
