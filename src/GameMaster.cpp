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
#include <database/Indexer.h>
#include <database/Database.h>
#include <unistd.h>
#include <stdlib.h>
#include <system_utils/SysUtils.h>

// Gameboards
#include <gameboards/GameBoard.h>

// Players
#include <players/RandomAi.h>
#include <players/LearningAi.h>

using namespace std;

void testIndexer();

static char threadSignal = 0;
static char threadFinished = 0;
void* execute_thread(void *arg)
{
   ThreadData *data = (ThreadData*)arg;
   long unsigned int count = 0;
   long unsigned int lastPost = 0;

   double start = SysUtils::getSecTime();
   while (count < data->numGames)
   {
      if (threadSignal == 1)
      {
         data->gm->executeGame(count+1);
         count++;
         long unsigned int tmp = count / data->printIncrement;
         if (tmp != lastPost)
         {
            fprintf(stderr,"Games Completed: %lu\n",count);
            lastPost = tmp;
         }
      }
      else if (threadSignal == 0)
      {
         count = data->numGames;
      }
   }
   double end = SysUtils::getSecTime();
   double avg = (end - start) / (double)count;
   fprintf(stderr,"Completed %lu games in %f sec\n",count,(end-start));
   fprintf(stderr,"Avg time per game = %f sec\n",avg);

   threadFinished = 1;
   return NULL;
}

GameMaster::GameMaster() :
   gameboard(NULL),
   players(),
   tid(),
   data(),
   efficiencyFilter(0.0f),
   suppressEfficiencyFilterPrint(false),
   statsOutput("")
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
   data.gm = this;
   data.numGames = -1;
   data.printIncrement = 1000;

   processCmdLine(argc,argv);

   gameboard = new GameBoard();
   if (playersToLoad.empty())
   {
      players.push_back(new RandomAi("AI_1"));
      players.push_back(new RandomAi("AI_2"));
      players.push_back(new RandomAi("AI_3"));
      players.push_back(new LearningAi("SMARTY"));
   }
   else
   {
      for (map<string,int>::iterator itr = playersToLoad.begin(); itr != playersToLoad.end(); ++itr)
      {
         switch (itr->second)
         {
            case 1:
               players.push_back(new LearningAi(itr->first));
               break;
            case 0:
            default:
               players.push_back(new RandomAi(itr->first));
               break;
         }
      }
   }

   if (!statsOutput.empty())
   {
      Logger::instance().openStatsFile(statsOutput.c_str(),playersToLoad);
   }

   printf("Registering players\n");
   for (Players::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      (*itr)->setState(gameboard->registerPlayer(*itr));
      (*itr)->setBoard(gameboard);
   }

   threadSignal = 2;
   kickOffThread();

   bool keepRunning = true;
   bool printHeader = true;
   while (keepRunning && !threadFinished)
   {
      if (printHeader)
      {
         fprintf(stderr,"Queued Games: %lu\n",data.numGames);
         fprintf(stderr,"Options:\n");
         fprintf(stderr,"   s - Start\n");
         fprintf(stderr,"   p - Pause\n");
         fprintf(stderr,"   q - Quit\n");
         printHeader = false;
      }
      char input = 0;
      scanf("%c",&input);
      switch (input)
      {
         case '\n':
            printHeader = true;
            break;
         case 's':
         case 'S':
            fprintf(stderr,"Starting Thread\n");
            threadSignal = 1;
            break;
         case 'p':
         case 'P':
            threadSignal = 2;
            fprintf(stderr,"Pausing Thread\n");
            break;
         case 'q':
         case 'Q':
         default:
            threadSignal = 0;
            keepRunning = false;
            fprintf(stderr,"Quiting\n");
            break;
      }
   }

   fprintf(stderr,"Waiting for thread to return\n");
   if (pthread_join(tid,NULL) != 0)
   {
      throw std::runtime_error("Failed to join thread.");
   }

//   testIndexer();

   fprintf(stderr,"Number of action permutations = %lu\n",Indexer::getNumActions());
}

void GameMaster::executeGame(long unsigned int gameId)
{
   printf("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   printf("~~~ STARTING GAME #%lu ~~~\n",gameId);
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

   recordGameboardCaches(gameId);
   printf("Finished game %lu\n",gameId);
}

void GameMaster::processCmdLine(int argc, char **argv)
{
   for (int i = 0; i < argc; ++i)
   {
      if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0)
      {
         printHelpMessage();
         exit(0);
      }
      else if (strcmp(argv[i],"-ao")==0 && (i+1) < argc)
      {
         if (!Logger::instance().openActionFile(argv[i+1]))
         {
            fprintf(stderr,"Failed to open output action file \"%s\"\n",argv[i+1]);
            throw std::runtime_error("Failed to open output action file.");
         }
      }
      else if (strcmp(argv[i],"-po")==0 && (i+1) < argc)
      {
         if (!Logger::instance().openPlanFile(argv[i+1]))
         {
            fprintf(stderr,"Failed to open output plan file \"%s\"\n",argv[i+1]);
            throw std::runtime_error("Failed to open output plan file.");
         }
      }
      else if (strcmp(argv[i],"-eo")==0 && (i+1) < argc)
      {
         if (!Logger::instance().openErrandFile(argv[i+1]))
         {
            fprintf(stderr,"Failed to open output errand file \"%s\"\n",argv[i+1]);
            throw std::runtime_error("Failed to open output errand file.");
         }
      }
      else if (strcmp(argv[i],"-so")==0 && (i+1) < argc)
      {
         statsOutput = argv[i+1];
      }
      else if (strcmp(argv[i],"-db")==0 && (i+1) < argc)
      {
         if (!Database::instance().loadDatabase(argv[i+1]))
         {
            fprintf(stderr,"Failed to open database file \"%s\"\n",argv[i+1]);
            throw std::runtime_error("Failed to open database file.");
         }
      }
      else if (strcmp(argv[i],"-rai")==0 && (i+1) < argc)
      {
         playersToLoad.insert(pair<string,int>(argv[i+1],0));
      }
      else if (strcmp(argv[i],"-lai")==0 && (i+1) < argc)
      {
         playersToLoad.insert(pair<string,int>(argv[i+1],1));
      }
      else if (strcmp(argv[i],"-n")==0 && (i+1) < argc)
      {
         sscanf(argv[i+1],"%lu",&data.numGames);
      }
      else if (strcmp(argv[i],"-ef")==0 && (i+1) < argc)
      {
         sscanf(argv[i+1],"%f",&efficiencyFilter);
      }
      else if (strcmp(argv[i],"-pi")==0 && (i+1) < argc)
      {
         sscanf(argv[i+1],"%lu",&data.printIncrement);
      }
      else if (strcmp(argv[i],"-sef")==0)
      {
         suppressEfficiencyFilterPrint = true;
      }
   }
}

void GameMaster::kickOffThread()
{
   pthread_attr_t attr;
   if (pthread_attr_init(&attr) != 0)
      throw std::runtime_error("Failed to initialize pthread attributes.");

   if (pthread_create(&tid,&attr,&execute_thread,&data) != 0)
      throw std::runtime_error("Failed to create thread.");

   if (pthread_attr_destroy(&attr) != 0)
      throw std::runtime_error("Failed to destroy thread attributes.");
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
         PlanData record;
         record.player = (*itr);
         record.round = gameboard->round;
         record.plan = plan;

         (*itr)->takePlan(plan);
         grantPlayerResources((*itr),plan);
         CardDeckTypes choices = (*itr)->chooseCardDraws(plan->cards);
         if (validateCardChoices(*itr,choices))
         {
            for (size_t i = 0; i < choices.size(); ++i)
            {
               Card::Ptr card = gameboard->decks[choices.at(i)]->draw();
//               if (card == NULL)
//                  throw std::runtime_error("DREW A NULL CARD!");
               record.cardsDrawn.push_back(card);
               givePlayerCard((*itr),card);
            }
            gameboard->planCache.push_back(record);
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
   printf("%s picked errand:\n",player->getName().c_str());
   errand->print();

   ErrandData record;
   record.player = player;
   record.round = gameboard->round;
   record.type = errand->type;
   record.card = errand->card;
   gameboard->errandCache.push_back(record);

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

void testIndexer()
{
   // testing indexer
   for (int round = 1; round <= 7; ++round)
   {
      for (int type = ActionType::Play; type <= ActionType::Discard; ++type)
      {
         for (int cid = CardId::Farm_1; cid <= CardId::Steward_S2; ++cid)
         {
            for (int mc = -8; mc <= 30; mc++)
            {
               int mcost = type == ActionType::Sell ? -1 * mc : mc;
               for (int acost = 0; acost <= 7; ++acost)
               {
                  for (int cc = 0; cc <= 10; ++cc)
                  {
                     int h,d,c,g;
                     switch (cc)
                     {
                        case 0:
                           h = 0;
                           d = 0;
                           c = 0;
                           g = 0;
                           break;
                        case 1:
                           h = 1;
                           d = 0;
                           c = 0;
                           g = 0;
                           break;
                        case 2:
                           h = 0;
                           d = 1;
                           c = 0;
                           g = 0;
                           break;
                        case 3:
                           h = 0;
                           d = 0;
                           c = 1;
                           g = 0;
                           break;
                        case 4:
                           h = 0;
                           d = 0;
                           c = 0;
                           g = 1;
                           break;
                        case 5:
                           h = 0;
                           d = 1;
                           c = 1;
                           g = 1;
                           break;
                        case 6:
                           h = 0;
                           d = 1;
                           c = 0;
                           g = 1;
                           break;
                        case 7:
                           h = 0;
                           d = 0;
                           c = 1;
                           g = 1;
                           break;
                        case 8:
                           h = 0;
                           d = 1;
                           c = 1;
                           g = 0;
                           break;
                        case 9:
                           h = 0;
                           d = 0;
                           c = 0;
                           g = 2;
                           break;
                        case 10:
                           h = 0;
                           d = 0;
                           c = 2;
                           g = 0;
                           break;
                        default:
                           break;
                     }

                     size_t ndx = Indexer::getActionNdx(
                        round, (ActionTypeType)type, (CardIdType)cid, mcost, acost, h, d, c, g);

                     int ro, to, cido, mo, ao, ho, dO, co, go;
                     Indexer::convertActionNdx(
                        ndx,ro,to,cido,mo,ao,ho,dO,co,go);
                     if (round != ro ||
                         type != to ||
                         cid != cido ||
                         mcost != mo ||
                         acost != ao ||
                         h != ho ||
                         d != dO ||
                         c != co ||
                         g != go)
                     {
                        fprintf(stderr,"%d %d %d %d %d %d %d %d %d\n",round,type,cid,mcost,acost,h,d,c,g);
                        fprintf(stderr,"  !=\n");
                        fprintf(stderr,"%d %d %d %d %d %d %d %d %d\n",ro,to,cido,mo,ao,ho,dO,co,go);
                        throw std::runtime_error("Indexer not converting ndx properly.");
                     }
                  }
               }
            }
         }
      }
   }
   fprintf(stderr,"Indexer seems to convert fine\n");
}

void GameMaster::printHelpMessage()
{
   fprintf(stderr,"Options:\n");
   fprintf(stderr,"   -n <number>\n      Number of games to complete before returning.\n      Defaults to (unsigned int) -1.\n");
   fprintf(stderr,"   -ao <file>\n      CSV/Text output file for action recordings.\n      Defaults to \"lw_actions.out\"\n");
   fprintf(stderr,"   -po <file>\n      CSV/Text output file for plan recordings.\n      Defaults to \"lw_plans.out\"\n");
   fprintf(stderr,"   -db <file>\n      File to use as action database. Defaults to\n      \"action_database.db\"\n");
}

void GameMaster::recordGameboardCaches(long unsigned int gameId)
{
   using namespace std;
   map<string,float> playerEfficiencies;
   bool efficiencyFilterMet = false;
   for (map<string,int>::iterator itr = gameboard->gameStats.endingMoney.begin(); itr != gameboard->gameStats.endingMoney.end(); ++itr)
   {
      float efficiency = (float)(gameboard->gameStats.startingMoney - itr->second) / (float)gameboard->gameStats.startingMoney;
      playerEfficiencies.insert(pair<string,float>(itr->first,efficiency));

      if (efficiency >= efficiencyFilter)
         efficiencyFilterMet = true;

      if (efficiencyFilter > 0.0f && efficiency >= efficiencyFilter)
         fprintf(stderr,"Player \"%s\" met the efficiency filter of %.02f with an efficiency of %.02f\n",itr->first.c_str(),efficiencyFilter,efficiency);
   }

   // Record Action Stats
   for (size_t ndx = 0; ndx < gameboard->actionCache.size(); ++ndx)
   {
      if (playerEfficiencies[gameboard->actionCache[ndx].player->getName()] >= efficiencyFilter)
      {
         bool won = gameboard->actionCache[ndx].player == gameboard->lastWinner;
         Logger::instance().recordAction(
            gameId,
            won,
            gameboard->actionCache[ndx].player->getName(),
            gameboard->actionCache[ndx].round,
            gameboard->actionCache[ndx].action);
         Database::instance().recordAction(
            won,
            gameboard->actionCache[ndx].round,
            gameboard->actionCache[ndx].action);
      }
   }

   // Record Plan Stats
   for (PlanDataSet::iterator itr = gameboard->planCache.begin(); itr != gameboard->planCache.end(); ++itr)
   {
      if (playerEfficiencies[itr->player->getName()] >= efficiencyFilter)
      {
         bool won = itr->player == gameboard->lastWinner;
         Logger::instance().recordPlan(
            gameId,
            won,
            itr->player->getName(),
            itr->round,
            itr->plan,
            itr->cardsDrawn);
         unsigned int properties = 0;
         unsigned int events = 0;
         unsigned int helpers = 0;
         unsigned int companions = 0;
         for (size_t i = 0; i < itr->cardsDrawn.size(); ++i)
         {
            switch (itr->cardsDrawn[i]->group)
            {
               case CardGroup::Property:
                  properties++;
                  break;
               case CardGroup::Event:
                  events++;
                  break;
               case CardGroup::HelperExpense:
                  helpers++;
                  break;
               case CardGroup::Companion:
                  companions++;
                  break;
               default:
                  break;
            }
         }
         Database::instance().recordDraw(won,itr->round,properties,events,helpers,companions);
      }
   }

   // Record Errand Stats
   for (ErrandDataSet::iterator itr = gameboard->errandCache.begin(); itr != gameboard->errandCache.end(); ++itr)
   {
      if (playerEfficiencies[itr->player->getName()] >= efficiencyFilter)
      {
         bool won = itr->player == gameboard->lastWinner;
         Logger::instance().recordErrand(
            gameId,
            won,
            itr->player->getName(),
            itr->round,
            itr->type,
            itr->card);
      }
   }

   // Record Game Stats
   if (efficiencyFilterMet)
   {
      if(!suppressEfficiencyFilterPrint)
         fprintf(stderr,"  Game %lu (Starting Money = %d) recorded\n",gameId,gameboard->gameStats.startingMoney);
      Logger::instance().recordStats(
         gameId,
         gameboard->gameStats.startingMoney,
         gameboard->gameStats.winner,
         gameboard->gameStats.endingMoney);
   }
}
