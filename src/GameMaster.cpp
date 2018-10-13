/*
 * GameMaster.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#include <GameMaster.h>
#include <stdexcept>
#include <Logging.h>
#include <Logger.h>
#include <string.h>

// Gameboards
#include <gameboards/GameBoard.h>

// Players
#include <players/RandomAi.h>

GameMaster::GameMaster() :
   gameboard(NULL),
   players()
{
   // Intentionally left blank
}

GameMaster::~GameMaster()
{
   if (gameboard != NULL)
   {
      delete gameboard;
   }
   for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      delete (*itr);
   }
}

void GameMaster::run(int argc, char **argv)
{
   processCmdLine(argc,argv);

   gameboard = new GameBoard();
   players.push_back(new RandomAi("AI_1"));
   players.push_back(new RandomAi("AI_2"));
   players.push_back(new RandomAi("AI_3"));

   printf("Registering players\n");
   for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      (*itr)->setState(gameboard->registerPlayer(*itr));
      (*itr)->setBoard(gameboard);
   }

   for (int i = 0; i < 1000; ++i)
   {
      printf("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      printf("~~~ STARTING GAME #%d ~~~\n",i+1);
      printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

      printf("Initializing gameboard\n");
      gameboard->initialize();

      printf("Starting game\n");
      while (gameboard->getNumRoundsRemaining() > 0)
      {
         printf("Starting round = %u\n",gameboard->round);

         // Setup Phase
         printf("~~~ Phase: Setup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         gameboard->executeSetup();
         gameboard->printState();
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

         // Planning Phase
         printf("~~~ Phase: Planning ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         planningPhase();
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

         // Errands Phase
         printf("~~~ Phase: Errands ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         errandsPhase();
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

         // Actions Phase
         printf("~~~ Phase: Actions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         actionsPhase();
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

         // End of Round Phase
         printf("~~~ Phase: End of Round ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
         gameboard->executeEndOfRound();
         printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
      }

      printf("All done\n");
      printf("Final Scores:\n");
      for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         std::string name = (*itr)->getName().c_str();
         printf("%s: $%d\n",name.c_str(),gameboard->players[name].state->resources[Resource::Money]);
      }
      printf("Player %s won!\n",gameboard->lastWinner->getName().c_str());

      for (size_t ndx = 0; ndx < gameboard->actionCache.size(); ++ndx)
      {
         bool won = gameboard->actionCache[ndx].player == gameboard->lastWinner;
         Logger::instance().recordAction(
            won,
            gameboard->actionCache[ndx].player->getName(),
            gameboard->actionCache[ndx].round,
            gameboard->actionCache[ndx].action);
      }
      fprintf(stderr,"Finished game %d\n",i);
   }
}

void GameMaster::processCmdLine(int argc, char **argv)
{
   for (int i = 0; i < argc; ++i)
   {
      if (strcmp(argv[i],"-o")==0 && (i+1) < argc)
      {
         if (!Logger::instance().openOutputFile(argv[i+1]))
         {
            fprintf(stderr,"Failed to open output file \"%s\"\n",argv[i+1]);
            throw std::runtime_error("Failed to open output file.");
         }
      }
   }
}

void GameMaster::planningPhase()
{
   // Get player order
   Players tmp;
   unsigned int ndx = gameboard->firstPlayer;
   printf("Player Planning Order:\n");
   for (size_t i = 0; i < players.size(); ++i)
   {
      printf("   %lu) %s\n",i+1,players[ndx]->getName().c_str());
      tmp.push_back(players[ndx]);
      ndx++;
      if (ndx >= players.size())
      {
         ndx = 0;
      }
   }

   // Have players pick plans
   for (Players::iterator itr = tmp.begin(); itr != tmp.end(); ++itr)
   {
      Plan::Ptr plan = (*itr)->pickPlan();
      if (plan != NULL && plan->location == NULL)
      {
         (*itr)->takePlan(plan);
         grantPlayerResources((*itr),plan);
         CardDeckTypes choices = (*itr)->chooseCardDraws(plan->cards);
         if (validateCardChoices(*itr,choices))
         {
            for (size_t i = 0; i < choices.size(); ++i)
            {
               givePlayerCard((*itr),gameboard->decks[choices.at(i)]->draw());
            }
         }
         else
         {
            throw std::runtime_error((*itr)->getName() +
                                     " picked an illegal set of cards to draw.");
         }
      }
      else
      {
         throw std::runtime_error((*itr)->getName() +
                                  " did not pick an acceptable plan.");
      }
      printf("%s picked plan %d\n",(*itr)->getName().c_str(),plan->priority);
      gameboard->printState();
   }
}

void GameMaster::errandsPhase()
{
   // Get new player order
   Players tmp;
   for (unsigned int i = 1; i <= gameboard->plans.size(); ++i)
   {
      for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         if ((*itr)->getPlan()->priority == i)
         {
            tmp.push_back(*itr);
         }
      }
   }

   // Have players run their errands
   for (int curErrand = 0; curErrand < 2; ++curErrand)
   {
      for (Players::iterator itr = tmp.begin(); itr != tmp.end(); ++itr)
      {
         if ((*itr)->getPlan()->errands > curErrand)
         {
            BoardSpot::Ptr errand = (*itr)->pickErrand();
            if (errand != NULL && !errand->occupied)
            {
               processErrand((*itr),errand);
            }
            else
            {
               throw std::runtime_error((*itr)->getName() +
                                        " chose an invalid errand.");
            }
         }
      }
   }
}

void GameMaster::actionsPhase()
{
   // Get player order
   Players tmp;
   for (unsigned int i = 1; i <= gameboard->plans.size(); ++i)
   {
      for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         if ((*itr)->getPlan()->priority == i)
         {
            tmp.push_back(*itr);
         }
      }
   }

   // Have players execute their actions
   for (size_t i = 0; i < tmp.size(); ++i)
   {
      tmp[i]->executeActions();

      // Print player state
      gameboard->players.find(tmp[i]->getName())->second.state->print();
   }
}

void GameMaster::givePlayerCard(PlayerInterface::Ptr player, Card::Ptr card)
{
   Resources &resources = gameboard->players[player->getName()].state->resources;
   if (card != NULL)
   {
      switch (card->type)
      {
         case CardType::Horse:
            resources[Resource::Horse] += 1;
            break;
         case CardType::Dog:
            resources[Resource::Dog] += 1;
            break;
         case CardType::Chef:
            resources[Resource::Chef] += 1;
            break;
         case CardType::Guest:
            resources[Resource::Guest] += 1;
            break;
         case CardType::WildCard:
            resources[Resource::WildCard] += 1;
            break;
         default:
            break;
      }
   }
   player->takeCard(card);
}

void GameMaster::grantPlayerResources(PlayerInterface::Ptr player, Plan::Ptr plan)
{
   Resources &resources = gameboard->players[player->getName()].state->resources;
   resources[Resource::Action] = plan->actions;
   // Boardspots and companion resources should've been set during
   // the gameboard's end of round cleanup after the discard down
   // phase.
//   Cards cards = gameboard->players[player->getName()].state->hand.getCards();
//   for (size_t i = 0; i < cards.size(); ++i)
//   {
//      switch (cards[i]->type)
//      {
//         case CardType::Horse:
//            resources[Resource::Horse] += 1;
//            break;
//         case CardType::Dog:
//            resources[Resource::Dog] += 1;
//            break;
//         case CardType::Chef:
//            resources[Resource::Chef] += 1;
//            break;
//         case CardType::Guest:
//            resources[Resource::Guest] += 1;
//            break;
//         default:
//            break;
//      }
//   }
//   resources[Resource::BoardSpot] = gameboard->players[player->getName()].state->board.getNumAvailSpots();
}

bool GameMaster::validateCardChoices(PlayerInterface::Ptr player, CardDeckTypes choices)
{
   bool choicesAreValid = true;

   // First check all the deck types
   for (size_t i = 0; i < choices.size(); ++i)
   {
      bool matchFound = false;
      for (size_t j = 0; j < CardDeck::Drawable.size(); ++j)
      {
         if (CardDeck::Drawable[j] == choices[i])
         {
            matchFound = true;
            break;
         }
      }
      if (!matchFound)
      {
         choicesAreValid = false;
         break;
      }
   }

   // Now compensate for special effects
   int numDraws = gameboard->players[player->getName()].state->plan->cards;
   PlayerBoard &board = gameboard->players[player->getName()].state->board;
   CardSpots cards = board.getOccupiedSpots();
   for (size_t i = 0; i < cards.size(); ++i)
   {
      switch (cards[i]->getCard()->effect)
      {
         case Effect::Draw2Cards:
            numDraws += 2;
            break;
         case Effect::Draw3Companions:
            removeAChoice(choices,CardDeck::Companions);
            removeAChoice(choices,CardDeck::Companions);
            removeAChoice(choices,CardDeck::Companions);
            break;
         case Effect::Draw3Events:
            removeAChoice(choices,CardDeck::Events);
            removeAChoice(choices,CardDeck::Events);
            removeAChoice(choices,CardDeck::Events);
            break;
         default:
            break;
      }
   }
   if (choices.size() > numDraws)
      choicesAreValid = false;

   return choicesAreValid;
}

void GameMaster::removeAChoice(CardDeckTypes &choices, CardDeckType type)
{
   for (CardDeckTypes::iterator itr = choices.begin(); itr != choices.end();++itr)
   {
      if ((*itr) == type)
      {
         choices.erase(itr);
         break;
      }
   }
}

void GameMaster::processErrand(PlayerInterface::Ptr player, BoardSpot::Ptr errand)
{
   switch (errand->type)
   {
      case Spot::PropertyMarket:
      {
         PropertyAdjustments adjusts = player->setPropertyAdjustments(gameboard->propertyMarket.getAdjustments());
         if (validatePropertyAdjustments(adjusts))
            gameboard->propertyMarket.setPropertyAdjustments(adjusts);
         else
            throw std::runtime_error(player->getName() +
                                     " set invalid property adjustments.");
         break;
      }
      case Spot::SingleDraw:
      {
         CardDeckTypes choices = player->chooseCardDraws(1,true);
         if (choices.size() == 1)
            givePlayerCard(player,gameboard->decks[choices.at(0)]->draw());
         else
            throw std::runtime_error(player->getName() +
                                     " wanted to draw more than a single card.");
         break;
      }
      case Spot::BoardExpansion:
         gameboard->players[player->getName()].state->board.extend();
         gameboard->players[player->getName()].state->resources[Resource::BoardSpot] += 1;
         break;
      case Spot::Opera:
      {
         Effects effects = player->activateExpenseEffects(CardType::Theatre);
         if (validatePlayerEffects(player,effects))
         {
            int expense = 2;
            for (int i = 0; i < effects.size(); ++i)
               if (effects[i] == Effect::TheatreSpend3Extra &&
                   gameboard->players[player->getName()].state->canSpendMoney(expense + 3))
                  expense += 3;
            gameboard->players[player->getName()].state->resources[Resource::Money] -= expense;
         }
         else
            throw std::runtime_error(player->getName() +
                                     " tried to activate effect not on board.");
         break;
      }
      case Spot::Card:
         givePlayerCard(player,errand->card);
         break;
      default:
         throw std::runtime_error(player->getName() +
                                  " attempted to process unrecognized BoardSpotType.");
         break;
   }

   // Mark the errand as occupied
   errand->occupied = true;
   errand->player = player->getName();
}

bool GameMaster::validatePropertyAdjustments(PropertyAdjustments adjusts)
{
   Adjustments values = gameboard->propertyMarket.getAdjustments();

   // Basic size check
   if (adjusts.size() != values.size())
      return false;

   // Adjusts use the available adjustments once for each
   bool adjustsAreValid = true;
   for (PropertyAdjustments::iterator adjust = adjusts.begin(); adjust != adjusts.end(); ++adjust)
   {
      bool matchFound = false;
      for (Adjustments::iterator itr = values.begin(); itr != values.end(); ++itr)
      {
         if (adjust->second == (*itr))
         {
            matchFound = true;
            values.erase(itr);
            break;
         }
      }

      if (!matchFound)
      {
         adjustsAreValid = false;
         break;
      }
   }

   return adjustsAreValid;
}

bool GameMaster::validatePlayerEffects(PlayerInterface::Ptr player, Effects effects)
{
   bool effectsValidated = true;

   Cards cards = gameboard->players[player->getName()].state->board.getCards();
   for (size_t i = 0; i < effects.size(); ++i)
   {
      bool matchFound = false;
      for (size_t j = 0; j < cards.size(); ++j)
      {
         if (cards[j]->effect == effects[i])
         {
            matchFound = true;
            break;
         }
      }

      if (!matchFound)
      {
         effectsValidated = false;
         break;
      }
   }

   return effectsValidated;
}
