/*
 * CardAttributes.cpp
 *
 *  Created on: Sep 15, 2018
 *      Author: milites
 */

#include <assets/cards/CardAttributes.h>

std::string CardId::toString(CardIdType id)
{
   switch (id)
   {
      case Invalid:
         return "Invalid";
         break;
      case Horse_1:
         return "Horse_1";
         break;
      case Horse_2:
         return "Horse_2";
         break;
      case Horse_3:
         return "Horse_3";
         break;
      case Horse_4:
         return "Horse_4";
         break;
      case Horse_5:
         return "Horse_5";
         break;
      case Dog_1:
         return "Dog_1";
         break;
      case Dog_2:
         return "Dog_2";
         break;
      case Dog_3:
         return "Dog_3";
         break;
      case Dog_4:
         return "Dog_4";
         break;
      case Dog_5:
         return "Dog_5";
         break;
      case Chef_1:
         return "Chef_1";
         break;
      case Chef_2:
         return "Chef_2";
         break;
      case Chef_3:
         return "Chef_3";
         break;
      case Chef_4:
         return "Chef_4";
         break;
      case Chef_5:
         return "Chef_5";
         break;
      case Guest_1:
         return "Guest_1";
         break;
      case Guest_2:
         return "Guest_2";
         break;
      case Guest_3:
         return "Guest_3";
         break;
      case Guest_4:
         return "Guest_4";
         break;
      case Guest_5:
         return "Guest_5";
         break;
      case Farm_1:
         return "Farm_1";
         break;
      case Farm_2:
         return "Farm_2";
         break;
      case Farm_3:
         return "Farm_3";
         break;
      case Farm_4:
         return "Farm_4";
         break;
      case Farm_5:
         return "Farm_5";
         break;
      case Farm_6:
         return "Farm_6";
         break;
      case Farm_7:
         return "Farm_7";
         break;
      case Farm_8:
         return "Farm_8";
         break;
      case Farm_9:
         return "Farm_9";
         break;
      case Farm_10:
         return "Farm_10";
         break;
      case Farm_11:
         return "Farm_11";
         break;
      case TownHouse_1:
         return "TownHouse_1";
         break;
      case TownHouse_2:
         return "TownHouse_2";
         break;
      case TownHouse_3:
         return "TownHouse_3";
         break;
      case TownHouse_4:
         return "TownHouse_4";
         break;
      case ManorHouse_1:
         return "ManorHouse_1";
         break;
      case ManorHouse_2:
         return "ManorHouse_2";
         break;
      case ManorHouse_3:
         return "ManorHouse_3";
         break;
      case ManorHouse_4:
         return "ManorHouse_4";
         break;
      case Mansion_1:
         return "Mansion_1";
         break;
      case Mansion_2:
         return "Mansion_2";
         break;
      case Mansion_3:
         return "Mansion_3";
         break;
      case Mansion_4:
         return "Mansion_4";
         break;
      case Mansion_5:
         return "Mansion_5";
         break;
      case CarriageRide_1:
         return "CarriageRide_1";
         break;
      case CarriageRide_2:
         return "CarriageRide_2";
         break;
      case CarriageRide_3:
         return "CarriageRide_3";
         break;
      case CarriageRide_4:
         return "CarriageRide_4";
         break;
      case CarriageRide_5:
         return "CarriageRide_5";
         break;
      case Theatre_1:
         return "Theatre_1";
         break;
      case Theatre_2:
         return "Theatre_2";
         break;
      case Theatre_3:
         return "Theatre_3";
         break;
      case Theatre_4:
         return "Theatre_4";
         break;
      case BoatTrip_1:
         return "BoatTrip_1";
         break;
      case BoatTrip_2:
         return "BoatTrip_2";
         break;
      case BoatTrip_3:
         return "BoatTrip_3";
         break;
      case BoatTrip_4:
         return "BoatTrip_4";
         break;
      case BoatTrip_5:
         return "BoatTrip_5";
         break;
      case BoatTrip_6:
         return "BoatTrip_6";
         break;
      case BoatTrip_7:
         return "BoatTrip_7";
         break;
      case Soiree_1:
         return "Soiree_1";
         break;
      case Soiree_2:
         return "Soiree_2";
         break;
      case Ball_1:
         return "Ball_1";
         break;
      case Ball_2:
         return "Ball_2";
         break;
      case Ball_3:
         return "Ball_3";
         break;
      case Dinner_1:
         return "Dinner_1";
         break;
      case Dinner_2:
         return "Dinner_2";
         break;
      case Dinner_3:
         return "Dinner_3";
         break;
      case Dinner_4:
         return "Dinner_4";
         break;
      case Dinner_5:
         return "Dinner_5";
         break;
      case Dinner_6:
         return "Dinner_6";
         break;
      case Dinner_7:
         return "Dinner_7";
         break;
      case Dinner_8:
         return "Dinner_8";
         break;
      case Dinner_9:
         return "Dinner_9";
         break;
      case Dinner_10:
         return "Dinner_10";
         break;
      case GentlemensClub_1:
         return "GentlemensClub_1";
         break;
      case GentlemensClub_2:
         return "GentlemensClub_2";
         break;
      case Reservation_1:
         return "Reservation_1";
         break;
      case Reservation_2:
         return "Reservation_2";
         break;
      case Reservation_3:
         return "Reservation_3";
         break;
      case Reservation_4:
         return "Reservation_4";
         break;
      case Reservation_5:
         return "Reservation_5";
         break;
      case Reservation_6:
         return "Reservation_6";
         break;
      case Carriage_1:
         return "Carriage_1";
         break;
      case Carriage_2:
         return "Carriage_2";
         break;
      case Carriage_3:
         return "Carriage_3";
         break;
      case Carriage_4:
         return "Carriage_4";
         break;
      case Carriage_5:
         return "Carriage_5";
         break;
      case TrainingGround_1:
         return "TrainingGround_1";
         break;
      case TrainingGround_2:
         return "TrainingGround_2";
         break;
      case TrainingGround_3:
         return "TrainingGround_3";
         break;
      case TrainingGround_4:
         return "TrainingGround_4";
         break;
      case TrainingGround_5:
         return "TrainingGround_5";
         break;
      case SeaDog:
         return "SeaDog";
         break;
      case Gardener_1:
         return "Gardener_1";
         break;
      case Gardener_2:
         return "Gardener_2";
         break;
      case Gardener_3:
         return "Gardener_3";
         break;
      case Valet:
         return "Valet";
         break;
      case EstateAgent_1:
         return "EstateAgent_1";
         break;
      case EstateAgent_2:
         return "EstateAgent_2";
         break;
      case EstateAgent_3:
         return "EstateAgent_3";
         break;
      case Waiter_1:
         return "Waiter_1";
         break;
      case Waiter_2:
         return "Waiter_2";
         break;
      case Waiter_3:
         return "Waiter_3";
         break;
      case SchoolChum_1:
         return "SchoolChum_1";
         break;
      case SchoolChum_2:
         return "SchoolChum_2";
         break;
      case SchoolChum_3:
         return "SchoolChum_3";
         break;
      case SchoolChum_4:
         return "SchoolChum_4";
         break;
      case Steward_1:
         return "Steward_1";
         break;
      case Steward_2:
         return "Steward_2";
         break;
      case Steward_3:
         return "Steward_3";
         break;
      case Tailor:
         return "Tailor";
         break;
      case Coachman_1:
         return "Coachman_1";
         break;
      case Coachman_2:
         return "Coachman_2";
         break;
      case BreedersFair:
         return "BreedersFair";
         break;
      case WildParty:
         return "WildParty";
         break;
      case HecticDay_1:
         return "HecticDay_1";
         break;
      case HecticDay_2:
         return "HecticDay_2";
         break;
      case HecticDay_3:
         return "HecticDay_3";
         break;
      case Ball_S:
         return "Ball_S";
         break;
      case CarriageRide_S:
         return "CarriageRide_S";
         break;
      case Dinner_S:
         return "Dinner_S";
         break;
      case OldFriend_1:
         return "OldFriend_1";
         break;
      case OldFriend_2:
         return "OldFriend_2";
         break;
      case OldFriend_3:
         return "OldFriend_3";
         break;
      case OldFriend_4:
         return "OldFriend_4";
         break;
      case SchoolChum_S:
         return "SchoolChum_S";
         break;
      case EstateAgent_S1:
         return "EstateAgent_S1";
         break;
      case EstateAgent_S2:
         return "EstateAgent_S2";
         break;
      case Steward_S1:
         return "Steward_S1";
         break;
      case Steward_S2:
         return "Steward_S2";
         break;
      case WildCard_1:
         return "WildCard_1";
         break;
      case WildCard_2:
         return "WildCard_2";
         break;
      case StartMoney_1:
         return "StartMoney_1";
         break;
      case StartMoney_2:
         return "StartMoney_2";
         break;
      case StartMoney_3:
         return "StartMoney_3";
         break;
      case StartMoney_4:
         return "StartMoney_4";
         break;
      case StartMoney_5:
         return "StartMoney_5";
         break;
      case StartMoney_6:
         return "StartMoney_6";
         break;
      case StartMoney_7:
         return "StartMoney_7";
         break;
      default:
         return "UNRECOGNIZED ID!";
         break;
   }
   return "SOME SHIT IS BROKE!";
}

std::string CardDeck::toString(CardDeckType type)
{
   switch (type)
   {
      case Invalid:
         return "Invalid";
         break;
      case Events:
         return "Events";
         break;
      case Properties:
         return "Properties";
         break;
      case HelpersExpenses:
         return "HelpersExpenses";
         break;
      case Companions:
         return "Companions";
         break;
      case Special:
         return "Special";
         break;
      case WildCards:
         return "WildCards";
         break;
      case LastWill:
         return "LastWill";
         break;
      case NumDeckTypes:
         return "NumDeckTypes";
         break;
      default:
         return "UNRECOGNIZED";
         break;
   }
}

std::string CardGroup::toString(CardGroupType type)
{
   switch (type)
   {
      case Invalid:
         return "Invalid";
         break;
      case Event:
         return "Event";
         break;
      case Property:
         return "Property";
         break;
      case HelperExpense:
         return "HelperExpense";
         break;
      case Companion:
         return "Companion";
         break;
      case Other:
         return "Other";
         break;
      default:
         return "UNRECOGNIZED";
         break;
   }
}

std::string CardType::toString(CardTypeType type)
{
   switch (type)
   {
      case Invalid:
         return "Invalid";
      case None:
         return "None";
      case Horse:
         return "Horse";
      case Dog:
         return "Dog";
      case Chef:
         return "Chef";
      case Guest:
         return "Guest";
      case Farm:
         return "Farm";
      case TownHouse:
         return "TownHouse";
      case ManorHouse:
         return "ManorHouse";
      case Mansion:
         return "Mansion";
      case Carriage:
         return "Carriage";
      case Theatre:
         return "Theatre";
      case BoatTrip:
         return "BoatTrip";
      case Party:
         return "Party";
      case Dinner:
         return "Dinner";
      case Helper:
         return "Helper";
      case TrainingGround:
         return "TrainingGround";
      case WildCard:
         return "WildCard";
      case LastWill:
         return "LastWill";
      default:
         return "UNRECOGNIZED";
   }
}

std::string CardName::toString(CardNameType type)
{
   switch (type)
   {
      case Invalid:
         return "Invalid";
      case Horse:
         return "Horse";
      case Dog:
         return "Dog";
      case Chef:
         return "Chef";
      case Guest:
         return "Guest";
      case Farm:
         return "Farm";
      case TownHouse:
         return "TownHouse";
      case ManorHouse:
         return "ManorHouse";
      case Mansion:
         return "Mansion";
      case CarriageRide:
         return "CarriageRide";
      case Theatre:
         return "Theatre";
      case BoatTrip:
         return "BoatTrip";
      case Soiree:
         return "Soiree";
      case Ball:
         return "Ball";
      case Dinner:
         return "Dinner";
      case GentlemensClub:
         return "GentlemensClub";
      case Reservation:
         return "Reservation";
      case Carriage:
         return "Carriage";
      case TrainingGround:
         return "TrainingGround";
      case SeaDog:
         return "SeaDog";
      case Gardener:
         return "Gardener";
      case Valet:
         return "Valet";
      case EstateAgent:
         return "EstateAgent";
      case Waiter:
         return "Waiter";
      case SchoolChum:
         return "SchoolChum";
      case Tailor:
         return "Tailor";
      case Coachman:
         return "Coachman";
      case BreedersFair:
         return "BreedersFair";
      case WildParty:
         return "WildParty";
      case HecticDay:
         return "HecticDay";
      case OldFriend:
         return "OldFriend";
      case WildCard:
         return "WildCard";
      case StartMoney:
         return "StartMoney";
      default:
         return "UNRECOGNIZED";
   }
}

std::string Effect::toString(EffectType type)
{
   switch (type)
   {
      case None:
         return "None";
         break;
      case Spend1PerFarmAndHorseOnFarm:
         return "Spend1PerFarmAndHorseOnFarm";
         break;
      case Spend2PerHorseOnFarm:
         return "Spend2PerHorseOnFarm";
         break;
      case Spend2PerFarm:
         return "Spend2PerFarm";
         break;
      case Spend1PerFarmAndDogOnFarm:
         return "Spend1PerFarmAndDogOnFarm";
         break;
      case Spend2PerDogOnFarm:
         return "Spend2PerDogOnFarm";
         break;
      case BoatTripAndPartySpend2Extra:
         return "BoatTripAndPartySpend2Extra";
         break;
      case TownAndManorUpkeep3Extra:
         return "TownAndManorUpkeep3Extra";
         break;
      case MansionAndManorUpkeep3Extra:
         return "MansionAndManorUpkeep3Extra";
         break;
      case Gain1EventAction:
         return "Gain1EventAction";
         break;
      case AdjustNonFarmValueBy2WhenBuySell:
         return "AdjustNonFarmValueBy2WhenBuySell";
         break;
      case DinnerSpend1Extra:
         return "DinnerSpend1Extra";
         break;
      case Plus2MaxHandSize:
         return "Plus2MaxHandSize";
         break;
      case MansionAndTownUpkeep3Extra:
         return "MansionAndTownUpkeep3Extra";
         break;
      case Perform1NonFarmUpkeep:
         return "Perform1NonFarmUpkeep";
         break;
      case Perform1FarmUpkeep:
         return "Perform1FarmUpkeep";
         break;
      case ReduceFarmValueBy2AndBuySell:
         return "ReduceFarmValueBy2AndBuySell";
         break;
      case Draw3Companions:
         return "Draw3Companions";
         break;
      case CarriageSpend1Extra:
         return "CarriageSpend1Extra";
         break;
      case CarriageSpend2Extra:
         return "CarriageSpend2Extra";
         break;
      case TheatreSpend3Extra:
         return "TheatreSpend3Extra";
         break;
      case Draw3Events:
         return "Draw3Events";
         break;
      case PlaceDogOrHorseOnFarmTwice:
         return "PlaceDogOrHorseOnFarmTwice";
         break;
      case DepreciateAHouseValue3Times:
         return "DepreciateAHouseValue3Times";
         break;
      case Gain1Action:
         return "Gain1Action";
         break;
      case Gain2Actions:
         return "Gain2Actions";
         break;
      case Gain3Actions:
         return "Gain3Actions";
         break;
      case Draw2Cards:
         return "Draw2Cards";
         break;
      case BuySell2NonFarms:
         return "BuySell2NonFarms";
         break;
      case Perform2FarmUpkeep:
         return "Perform2FarmUpkeep";
         break;
      case ReduceNonFarmBy4WhenBuySell:
         return "ReduceNonFarmBy4WhenBuySell";
         break;
      default:
         return "UNRECOGNIZED EFFECT!";
         break;
   }
   return "SOME EFFECT SHIT IS BROKE!";
}

Effects Effect::getEffectsRelatedToPlacing(CardTypeType type)
{
   Effects effects;
   switch (type)
   {
      case CardType::Farm:
         effects.push_back(ReduceFarmValueBy2AndBuySell);
         break;
      case CardType::TownHouse:
      case CardType::ManorHouse:
      case CardType::Mansion:
         effects.push_back(AdjustNonFarmValueBy2WhenBuySell);
         effects.push_back(BuySell2NonFarms);
         effects.push_back(ReduceNonFarmBy4WhenBuySell);
         break;
      default:
         break;
   }
   return effects;
}

Effects Effect::getEffectsRelatedToExpense(CardTypeType type)
{
   Effects effects;
   switch (type)
   {
      case CardType::Farm:
         effects.push_back(Perform1FarmUpkeep);
         effects.push_back(Perform2FarmUpkeep);
         break;
      case CardType::TownHouse:
         effects.push_back(TownAndManorUpkeep3Extra);
         effects.push_back(MansionAndTownUpkeep3Extra);
         effects.push_back(Perform1NonFarmUpkeep);
         break;
      case CardType::ManorHouse:
         effects.push_back(TownAndManorUpkeep3Extra);
         effects.push_back(MansionAndManorUpkeep3Extra);
         effects.push_back(Perform1NonFarmUpkeep);
         break;
      case CardType::Mansion:
         effects.push_back(MansionAndManorUpkeep3Extra);
         effects.push_back(MansionAndTownUpkeep3Extra);
         effects.push_back(Perform1NonFarmUpkeep);
         break;
      case CardType::Carriage:
         effects.push_back(CarriageSpend1Extra);
         effects.push_back(CarriageSpend2Extra);
         break;
      case CardType::Theatre:
         effects.push_back(TheatreSpend3Extra);
         break;
      case CardType::BoatTrip:
         effects.push_back(BoatTripAndPartySpend2Extra);
         break;
      case CardType::Party:
         effects.push_back(BoatTripAndPartySpend2Extra);
         break;
      case CardType::Dinner:
         effects.push_back(DinnerSpend1Extra);
         break;
      default:
         break;
   }
   return effects;
}

Effects Effect::getEffectsWithLimitedActivation()
{
   Effects effects;
   effects.push_back(Effect::Gain1EventAction);
   effects.push_back(Effect::Gain1Action);
   effects.push_back(Effect::Gain2Actions);
   effects.push_back(Effect::Gain3Actions);
   effects.push_back(Effect::BuySell2NonFarms);
   effects.push_back(Effect::Perform2FarmUpkeep);
   effects.push_back(Effect::ReduceFarmValueBy2AndBuySell);
   effects.push_back(Effect::Perform1FarmUpkeep);
   effects.push_back(Effect::Perform1NonFarmUpkeep);
   return effects;
}

std::string Resource::toString(ResourceType type)
{
   std::string value = "Unrecognized";
   switch (type)
   {
      case Invalid:
         value = "Invalid";
         break;
      case Base:
         value = "Base";
         break;
      case Money:
         value = "Money";
         break;
      case BoardSpot:
         value = "BoardSpot";
         break;
      case Action:
         value = "Action";
         break;
      case EventAction:
         value = "EventAction";
         break;
      case FarmUpkeepAction:
         value = "FarmUpkeepAction";
         break;
      case NonFarmUpkeepAction:
         value = "NonFarmUpkeepAction";
         break;
      case BuySellFarmAction:
         value = "BuySellFarmAction";
         break;
      case BuySellNonFarmAction:
         value = "BuySellNonFarmAction";
         break;
      case SellNonFarmAction:
         value = "SellNonFarmAction";
         break;
      case Horse:
         value = "Horse";
         break;
      case Dog:
         value = "Dog";
         break;
      case Chef:
         value = "Chef";
         break;
      case Guest:
         value = "Guest";
         break;
      case WildCard:
         value = "WildCard";
         break;
      case ActionAnd2Money:
         value = "ActionAnd2Money";
         break;
      default:
         break;
   }
   return value;
}
