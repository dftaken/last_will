/*
 * RandomAi.cpp
 *
 *  Created on: Sep 15, 2018
 *      Author: milites
 */

#include <players/RandomAi.h>
#include <system_utils/SysUtils.h>
#include <stdexcept>
#include <Logging.h>

using namespace std;

RandomAi::RandomAi(std::string name) :
   PlayerInterface(),
   myName(name),
   myState(NULL),
   gameboard(NULL)
{
   // Intentionally left blank
}

RandomAi::~RandomAi()
{
   // Intentionally left blank
}

string RandomAi::getName()
{
   return myName;
}

void RandomAi::setState(PlayerState::Ptr state)
{
   myState = state;
}

void RandomAi::setBoard(GameBoardInterface::Ptr board)
{
   gameboard = board;
}

Cards RandomAi::discardCards(unsigned int numToDiscard)
{
   Cards toDiscard;
   while (numToDiscard > 0)
   {
      Cards cards = myState->hand.getCards();
      size_t ndx = SysUtils::getRandL() % cards.size();
      toDiscard.push_back(cards.at(ndx));
      numToDiscard--;
   }
   return toDiscard;
}

Cards RandomAi::depreciateThreeProperties()
{
   // Determine eligible properties
   Cards properties;
   Cards boardCards = myState->board.getCards();
   for (size_t i = 0; i < boardCards.size(); ++i)
   {
      if (boardCards[i]->group == CardGroup::Property &&
          boardCards[i]->name != CardName::Farm)
      {
         properties.push_back(boardCards[i]);
      }
   }

   // Pick 3 properties
   Cards choices;
   if (properties.size() > 0)
   {
      for (size_t i = 0; i < 3; ++i)
      {
         size_t ndx = SysUtils::getRandL() % properties.size();
         choices.push_back(properties[ndx]);
      }
   }

   return choices;
}

Cards RandomAi::placeTwoDogsHorses()
{
   // Determine eligible properties
   Cards properties;
   Cards boardCards = myState->board.getCards();
   for (size_t i = 0; i < boardCards.size(); ++i)
   {
      if (boardCards[i]->name == CardName::Farm)
      {
         unsigned int nextUpgrade = boardCards[i]->curLevel + 1;
         if (nextUpgrade < boardCards[i]->upgradeLevels.size())
         {
            ResourceType cost = boardCards[i]->upgradeLevels[nextUpgrade].cost;
            if (myState->resources[cost] > 0)
            {
               properties.push_back(boardCards[i]);
            }
         }
      }
   }

   // Pick 2 properties
   Cards choices;
   if (properties.size() > 0)
   {
      for (size_t i = 0; i < 2; ++i)
      {
         size_t ndx = SysUtils::getRandL() % properties.size();
         choices.push_back(properties[ndx]);
      }
   }

   return choices;
}

CardDeckTypes RandomAi::chooseCardDraws(int numDraws, bool hardLimit)
{
   CardDeckTypes choices;
   if (!hardLimit)
      choices = useAllCardDrawEffects(numDraws);
   for (int i = 0; i < numDraws; ++i)
   {
      unsigned int ndx = SysUtils::getRandL() % CardDeck::Drawable.size();
      choices.push_back(CardDeck::Drawable.at(ndx));
   }
   return choices;
}

CardDeckTypes RandomAi::useAllCardDrawEffects(int &numDraws)
{
   CardDeckTypes choices;
   CardSpots cards = myState->board.getOccupiedSpots();
   for (CardSpots::iterator itr = cards.begin(); itr != cards.end(); ++itr)
   {
      Card::Ptr card = (*itr)->getCard();
      switch (card->effect)
      {
         case Effect::Draw2Cards:
            numDraws += 2;
            break;
         case Effect::Draw3Companions:
            choices.push_back(CardDeck::Companions);
            choices.push_back(CardDeck::Companions);
            choices.push_back(CardDeck::Companions);
            break;
         case Effect::Draw3Events:
            choices.push_back(CardDeck::Events);
            choices.push_back(CardDeck::Events);
            choices.push_back(CardDeck::Events);
            break;
         default:
            break;
      }
   }
   return choices;
}

void RandomAi::takeCard(Card::Ptr card)
{
   myState->hand.addCard(card);
}

Plan::Ptr RandomAi::pickPlan()
{
   Plans avail = gameboard->getAvailablePlans();
   size_t ndx = SysUtils::getRandL() % avail.size();
   return avail.at(ndx);
}

void RandomAi::takePlan(Plan::Ptr plan)
{
   myState->takePlan(plan);
}

Plan::Ptr RandomAi::getPlan()
{
   return myState->plan;
}

BoardSpot::Ptr RandomAi::pickErrand()
{
   printf("\n%s::pickErrand()\n",getName().c_str());
   BoardSpots avail = gameboard->getAvailableErrands(this);
   size_t ndx = SysUtils::getRandL() % avail.size();
   return avail.at(ndx);
}

PropertyAdjustments RandomAi::setPropertyAdjustments(Adjustments adjustments)
{
   PropertyAdjustments result;
   for (size_t i = adjustments.size(); i > 0; --i)
   {
      size_t ndx = SysUtils::getRandL() % i;
      switch (i)
      {
         case 4:
            result[CardName::Farm] = adjustments.at(ndx);
            break;
         case 3:
            result[CardName::TownHouse] = adjustments.at(ndx);
            break;
         case 2:
            result[CardName::ManorHouse] = adjustments.at(ndx);
            break;
         case 1:
            result[CardName::Mansion] = adjustments.at(ndx);
            break;
         default:
            break;
      }
      adjustments.erase(adjustments.begin()+ndx);
   }
   return result;
}

Effects RandomAi::activateExpenseEffects(CardTypeType type)
{
   return getAllExpenseEffects(type);
}

Effects RandomAi::getAllExpenseEffects(CardTypeType type)
{
   Effects effects = Effect::getEffectsRelatedToExpense(type);
   Cards cards = myState->board.getCards();
   for (Effects::iterator itr = effects.begin(); itr != effects.end();)
   {
      bool matchFound = false;

      for (size_t i = 0; i < cards.size(); ++i)
      {
         if (cards[i]->effect == (*itr))
         {
            matchFound = true;
            break;
         }
      }

      if (!matchFound)
         itr = effects.erase(itr);
      else
         ++itr;
   }
   return effects;
}

void RandomAi::executeActions()
{
   printf("\n--- %s::executeActions() START\n",getName().c_str());
   myState->print();
   Actions options = gameboard->getAvailableActions(this);
   for (Actions::iterator itr = options.begin(); itr != options.end();)
   {
      if (itr->type == ActionType::Discard)
         itr = options.erase(itr);
      else
         ++itr;
   }

   while (options.size() > 0)
   {
      printf("options:\n");
      options.print();

      size_t ndx = SysUtils::getRandL() % options.size();
      printf("%s choosing action: %lu\n",getName().c_str(),ndx);
      if (!gameboard->processAction(this,options[ndx]))
      {
         throw std::runtime_error(
            "Something went wrong trying to process an action.");
      }
      printf("Action processed.\n\n");

      // Get new set of possible actions and cull out discard options
      options = gameboard->getAvailableActions(this);
      for (Actions::iterator itr = options.begin(); itr != options.end();)
      {
         if (itr->type == ActionType::Discard)
            itr = options.erase(itr);
         else
            ++itr;
      }
      myState->print();
   }
   printf("--- %s::executeActions() END\n",getName().c_str());
}

void RandomAi::print()
{
   printf("%s:\n",myState->name.c_str());
   printf("   $%d\n",myState->resources[Resource::Money]);
   Cards hand = myState->hand.getCards();
   printf("   Hand:\n");
   for (Cards::iterator itr = hand.begin(); itr != hand.end(); ++itr)
   {
      printf("      %s\n",(*itr)->strName.c_str());
   }
}
