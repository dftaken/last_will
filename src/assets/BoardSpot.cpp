/*
 * BoardSpot.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: milites
 */

#include <assets/BoardSpot.h>
#include <Logging.h>

#include <stdexcept>

using namespace std;

string Spot::toString(SpotType type)
{
   switch (type)
   {
      case Invalid:
         return "Invalid";
         break;
      case PropertyMarket:
         return "PropertyMarket";
         break;
      case BoardExpansion:
         return "BoardExpansion";
         break;
      case Opera:
         return "Opera";
         break;
      case SingleDraw:
         return "SingleDraw";
         break;
      case Card:
         return "Card";
         break;
      default:
         return "Unrecognized Type";
         break;
   }
   return "Shit gone sideways";
}

BoardSpot::BoardSpot() :
   type(Spot::Invalid),
   occupied(false),
   player(),
   card(NULL),
   decks()
{
   // Intentionally left blank
}

BoardSpot::BoardSpot(SpotType inType) :
   type(inType),
   occupied(false),
   player(),
   card(NULL),
   decks()
{
   // Intentionally left blank
}

BoardSpot::BoardSpot(SpotType inType, CardDeckType deckType) :
   type(inType),
   occupied(false),
   player(),
   card(NULL),
   decks()
{
   for (int i = 0; i < 7; ++i)
   {
      decks.push_back(deckType);
   }
}

BoardSpot::BoardSpot(SpotType inType, CardDeckType d1, CardDeckType d2,
                     CardDeckType d3, CardDeckType d4, CardDeckType d5,
                     CardDeckType d6, CardDeckType d7) :
   type(inType),
   occupied(false),
   player(),
   card(NULL),
   decks()
{
   decks.push_back(d1);
   decks.push_back(d2);
   decks.push_back(d3);
   decks.push_back(d4);
   decks.push_back(d5);
   decks.push_back(d6);
   decks.push_back(d7);
}

BoardSpot::~BoardSpot()
{
   // Intentionally left blank
}

std::string BoardSpot::getName()
{
   string name;
   switch (type)
   {
      case Spot::PropertyMarket:
         name = "PropertyMarketSpot";
         break;
      case Spot::BoardExpansion:
         name = "BoardExpansionSpot";
         break;
      case Spot::Opera:
         name = "TheatreSpot";
         break;
      case Spot::SingleDraw:
         name = "SingleDrawSpot";
         break;
      case Spot::Card:
         name = "CardSpot";
         break;
      default:
         name = "Invalid";
         break;
   }
   return name;
}

void BoardSpot::releaseCard(Card::Ptr ptr)
{
   if (card == ptr && card != NULL)
   {
      card->location = NULL;
      card = NULL;
   }
}

void BoardSpot::releaseCard(Card &ref)
{
   releaseCard(&ref);
}

void BoardSpot::releaseCard(CardIdType id)
{
   if (card != NULL && card->id == id)
   {
      releaseCard(card);
   }
}

void BoardSpot::setCard(Card::Ptr ptr)
{
   if (ptr != NULL && card == NULL)
   {
      if (ptr->location != NULL)
      {
         ptr->location->releaseCard(ptr);
      }
      ptr->location = this;
      card = ptr;
   }
   else if (card != NULL)
   {
      throw std::runtime_error(
         "Attempted to place a card on an occupied spot.");
   }
}

void BoardSpot::executeEndOfRound()
{
   occupied = false;
   player.clear();
}

void BoardSpot::reset()
{
   executeEndOfRound();
   releaseCard();
}

void BoardSpot::releaseCard()
{
   releaseCard(card);
}

void BoardSpot::print()
{
   printf("BoardSpot:\n");
   printf("   Type: %s\n",Spot::toString(type).c_str());
   printf("   Occupied: %d\n",occupied);
   printf("   Player: %s\n",player.c_str());
   if (card != NULL)
      printf("   Card: %s\n",CardId::toString(card->id).c_str());
   else
      printf("   Card: None\n");
//   printf("   Next Deck: %")
}
