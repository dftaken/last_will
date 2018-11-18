/*
 * Deck.cpp
 *
 *  Created on: Sep 8, 2018
 *      Author: milites
 */

#include <assets/Deck.h>
#include <assets/cards/CardLibrary.h>
#include <system_utils/SysUtils.h>
#include <Logging.h>
#include <stdexcept>

using namespace std;

Deck::Deck(CardDeckType inType) :
   type(inType),
   discardPile(),
   drawPile()
{
   reset();
}

Deck::~Deck()
{
   // Intentionally left blank
}

/*
 * Deck Functions
 */
void Deck::reset()
{
   typedef CardLibrary::Database Database;
   const Database db = CardLibrary::getInstance()->getDatabase();
   for (Database::const_iterator itr = db.begin(); itr != db.end(); ++itr)
   {
      Card::Ptr const &card = itr->second;
      if (card->deck == type)
      {
         if (card->location != NULL)
         {
            card->location->releaseCard(card);
         }
         card->location = this;
         drawPile.push_back(card);
      }
   }
   shuffle();
}

void Deck::shuffle()
{
   // Copy all cards into the temp stack
   Cards tmp = drawPile;
   for (Cards::iterator itr = discardPile.begin(); itr != discardPile.end(); ++itr)
   {
      tmp.push_back(*itr);
   }
   discardPile.clear();

   // Randomly add the cards back into the drawPile
   drawPile.clear();
   if (type != CardDeck::Special)
   {
      for (int i = tmp.size(); i > 0; --i)
      {
         int ndx = SysUtils::getRandL() % i;
         drawPile.push_back(tmp.at(ndx));
         tmp.erase(tmp.begin()+ndx);
      }
   }
   else
   {
      Cards crown1;
      Cards crown2;
      Cards crown3;
      for (int i = tmp.size(); i > 0; --i)
      {
         int ndx = SysUtils::getRandL() % i;
         switch (tmp.at(ndx)->crowns)
         {
            case 1:
               crown1.push_back(tmp.at(ndx));
               break;
            case 2:
               crown2.push_back(tmp.at(ndx));
               break;
            case 3:
               crown3.push_back(tmp.at(ndx));
               break;
            default:
               break;
         }
         tmp.erase(tmp.begin()+ndx);
      }
      for (Cards::iterator itr = crown3.begin(); itr != crown3.end(); ++itr)
      {
         drawPile.push_back(*itr);
      }
      for (Cards::iterator itr = crown2.begin(); itr != crown2.end(); ++itr)
      {
         drawPile.push_back(*itr);
      }
      for (Cards::iterator itr = crown1.begin(); itr != crown1.end(); ++itr)
      {
         drawPile.push_back(*itr);
      }
   }
}

Card::Ptr Deck::draw()
{
   if (drawPile.size() == 0 && type != CardDeck::Special)
   {
      shuffle();
      if (drawPile.size() <= 0)
      {
         CardLibrary::Database cards = CardLibrary::getInstance()->getDatabase();
         for (CardLibrary::Database::iterator itr = cards.begin(); itr != cards.end(); ++itr)
         {
            if (itr->second->deck == type)
            {
               fprintf(stderr,"%s located at %s\n",
                       CardId::toString(itr->second->id).c_str(),
                       itr->second->location->getName().c_str());
            }
         }
         throw std::runtime_error("Shuffled but still no draw pile!");
      }
   }

   Card::Ptr output = NULL;
   if (drawPile.size() > 0)
   {
      output = drawPile.back();
      drawPile.pop_back();
      output->location = NULL;
   }

   return output;
}

void Deck::discard(Card::Ptr card)
{
   // Make sure the previous location has release the card
   if (card != NULL && card->location != NULL)
   {
      card->location->releaseCard(card);
   }

   // Update the card's location and add it to the discardPile
   card->location = this;
   discardPile.push_back(card);
}

size_t Deck::getDrawSize()
{
   return drawPile.size();
}

size_t Deck::getDiscardSize()
{
   return discardPile.size();
}

/*
 * Location Functions
 */
string Deck::getName()
{
   string name = string("");
   switch (type)
   {
      case CardDeck::Invalid:
         name = string("Invalid");
         break;
      case CardDeck::Events:
         name = string("Events");
         break;
      case CardDeck::Properties:
         name = string("Properties");
         break;
      case CardDeck::HelpersExpenses:
         name = string("HelpersExpenses");
         break;
      case CardDeck::Companions:
         name = string("Companions");
         break;
      case CardDeck::Special:
         name = string("Special");
         break;
      case CardDeck::WildCards:
         name = string("WildCards");
         break;
      case CardDeck::LastWill:
         name = string("LastWill");
         break;
      default:
         name = string("UNDEFINED");
         break;
   }
   return name;
}

void Deck::releaseCard(Card::Ptr card)
{
   if (card != NULL)
   {
      releaseCard(card->id);
   }
}

void Deck::releaseCard(Card &card)
{
   releaseCard(card.id);
}

void Deck::releaseCard(CardIdType id)
{
   bool cardFound = false;
   for (Cards::iterator itr = discardPile.begin(); itr != discardPile.end(); ++itr)
   {
      if ((*itr)->id == id)
      {
         (*itr)->location = NULL;
         discardPile.erase(itr);
         cardFound = true;
         break;
      }
   }
   if (!cardFound)
   {
      for (Cards::iterator itr = drawPile.begin(); itr != drawPile.end(); ++itr)
      {
         if ((*itr)->id == id)
         {
            (*itr)->location = NULL;
            drawPile.erase(itr);
            cardFound = true;
            break;
         }
      }
   }
}
