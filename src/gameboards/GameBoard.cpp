/*
 * GameBoard.cpp
 *
 *  Created on: Sep 8, 2018
 *      Author: milites
 */

#include <gameboards/GameBoard.h>

#include <assets/cards/CardAttributes.h>
#include <system_utils/SysUtils.h>
#include <Logging.h>

#include <stdexcept>

using namespace std;

GameBoard::GameBoard() :
   GameBoardInterface(),
   decks(),
   players(),
   firstPlayer(0),
   round(0),
   plans(),
   propertyMarket(),
   marketSpot(Spot::PropertyMarket),
   singleCardDraws(),
   boardExtensions(),
   numBoardExtensions(0),
   operaSpot(Spot::Opera),
   cardSpots()
{
   // Construct decks
   for (int i = 0; i < CardDeck::NumDeckTypes; ++i)
   {
      CardDeckType deckType = static_cast<CardDeckType>(i);
      decks.insert(DeckEntry(deckType,new Deck(deckType)));
   }
}

GameBoard::~GameBoard()
{
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      delete itr->second.state;
   }
   for (Plans::iterator itr = plans.begin(); itr != plans.end(); ++itr)
   {
      delete (*itr);
   }
   for (Decks::iterator itr = decks.begin(); itr != decks.end(); ++itr)
   {
      delete itr->second;
   }
}

/*
 * Game Board Interface Functions
 */
Plans GameBoard::getAvailablePlans()
{
   Plans available;
   for (Plans::iterator itr = plans.begin(); itr != plans.end(); ++itr)
   {
      if ((*itr)->location == NULL)
      {
         available.push_back(*itr);
      }
   }
   if (available.size() == 0)
   {
      throw std::runtime_error("No plans available.");
   }
   return available;
}

Plans GameBoard::getAllPlans()
{
   return plans;
}

BoardSpots GameBoard::getAvailableErrands(GamePlayer::Ptr player)
{
   if (player == NULL)
      throw std::runtime_error("player is NULL.");

   BoardSpots errands;

   if (!marketSpot.occupied)
      errands.push_back(&marketSpot);
   if (!singleCardDraws[player->getName()].occupied)
      errands.push_back(&singleCardDraws[player->getName()]);
   for (BoardSpotObjects::iterator itr = boardExtensions.begin(); itr != boardExtensions.end(); ++itr)
   {
      if (!itr->occupied)
         errands.push_back(&(*itr));
   }
   if (!operaSpot.occupied && players[player->getName()].state->canSpendMoney(2))
      errands.push_back(&operaSpot);
   if (getNumPlayers() == 2 || getNumPlayers() == 3)
   { // Special case for the first 3 spots (Special Card spots)
      // The first 3 card slots are shared by 2 errand spots
      int numOccupied = cardSpots[0].occupied ? 1 : 0;
      numOccupied += cardSpots[1].occupied ? 1 : 0;
      numOccupied += cardSpots[2].occupied ? 1 : 0;
      if (numOccupied > 2)
      {
         throw std::runtime_error("Special card errands have exceeded errands.");
      }
      else if (numOccupied < 2)
      {
         for (int i = 0; i < 3; ++i)
         {
            if (!cardSpots[i].occupied)
               errands.push_back(&cardSpots[i]);
         }
      }
      for (size_t i = 3; i < cardSpots.size(); ++i)
      {
         if (!cardSpots[i].occupied)
            errands.push_back(&cardSpots[i]);
      }
   }
   else
   { // All spots are independent
      for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
      {
         if (!itr->occupied)
            errands.push_back(&(*itr));
      }
   }

   return errands;
}

BoardSpots GameBoard::getAllErrands(GamePlayer::Ptr player)
{
   if (player == NULL)
      throw std::runtime_error("player is NULL.");

   BoardSpots errands;
   errands.push_back(&marketSpot);
   errands.push_back(&singleCardDraws[player->getName()]);
   for (BoardSpotObjects::iterator itr = boardExtensions.begin(); itr != boardExtensions.end(); ++itr)
      errands.push_back(&(*itr));
   errands.push_back(&operaSpot);
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
      errands.push_back(&(*itr));

   return errands;
}

Actions GameBoard::getAvailableActions(GamePlayer::Ptr player)
{
   PlayerState::Ptr state = players[player->getName()].state;
   Actions actions;

   // Add all discard actions first
   Cards boardCards = state->board.getCards();
   for (size_t i = 0; i < boardCards.size(); ++i)
   {
      if (boardCards[i]->group != CardGroup::Property)
      {
         Action action(ActionType::Discard,boardCards[i]);
         action.cost[Resource::BoardSpot] = -1; // Return a board spot
         actions.push_back(action);
      }
   }

   // Add Play Actions
   addPlayActionPermutations(player,actions);

   // Add Upgrade Actions
   addUpgradePermutations(player,actions);

   // Add Expense Actions
   addExpensePermutations(player,actions);

   // Add Activate Actions
   addActivatePermutations(player,actions);

   // Add Sell Actions
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      Card::Ptr card = boardSpots[i]->getCard();
      if (card != NULL && card->group == CardGroup::Property)
      {
         addPropertySellPermutations(player,card,actions);
      }
   }

   // Remove any actions the player can't afford
   for (Actions::iterator itr = actions.begin(); itr != actions.end();)
   {
      if (!playerCanPay(player,(*itr)))
      {
         itr = actions.erase(itr);
      }
      else
      {
         ++itr;
      }
   }

   return actions;
}

bool GameBoard::processAction(GamePlayer::Ptr player, Action action)
{
   bool result = false;
   switch (action.type)
   {
      case ActionType::Play:
         printf("Processing Play Action\n");
         result = processPlayAction(player,action);
         break;
      case ActionType::Upgrade:
         printf("Processing Upgrade Action\n");
         result = processUpgradeAction(player,action);
         break;
      case ActionType::Expense:
         printf("Processing Expense Action\n");
         result = processExpenseAction(player,action);
         break;
      case ActionType::Activate:
         printf("Processing Activate Action\n");
         result = processActivateAction(player,action);
         break;
      case ActionType::Sell:
         printf("Processing Sell Action\n");
         result = processSellAction(player,action);
         break;
      case ActionType::Discard:
         printf("Processing Discard Action\n");
         result = processDiscardAction(player,action);
         break;
      default:
         throw std::runtime_error("Attempted to process unrecognized action type.");
         break;
   }
   printf("Done processing action\n");

   // Cache the action
   ActionData data;
   data.player = player;
   data.round = round;
   data.action = action;
   actionCache.push_back(data);

   return result;
}

/*
 * Game Master Functions
 */
PlayerState::Ptr GameBoard::registerPlayer(GamePlayer::Ptr player)
{
   if (player != NULL)
   {
      if (players.find(player->getName()) == players.end())
      {
         PlayerState::Ptr state = new PlayerState(player->getName());
         PlayerData data;
         data.player = player;
         data.state = state;
         players.insert(PlayerStateEntry(player->getName(),data));
         return state;
      }
      else
      {
         throw std::runtime_error(
            player->getName() + string(" is already registered. ") +
            string("All player names must be unique."));
      }
      return NULL;
   }
   else
   {
      throw std::runtime_error("Attempted to register NULL player.");
   }
   return NULL;
}

void GameBoard::initialize()
{
   gameStats.winner.clear();
   gameStats.startingMoney = 0;
   gameStats.endingMoney.clear();

   // Check to make sure we have a valid number of players
   getNumPlayers();

   // Reset the decks
   for (Decks::iterator itr = decks.begin(); itr != decks.end(); ++itr)
   {
      itr->second->reset();
   }

   // Randomly choose first player
   firstPlayer = SysUtils::getRandL() % players.size();

   // Reset the round tracker
   round = 1;

   // Initialize Player States
   initPlayers();

   // Initialize board state
   initPlans();
   initPropertyMarket();
   initSingleCardDraws();
   initBoardExtensions();
   initOperaSpot();
   initCardSpots();

   // Clear caches
   actionCache.clear();
   planCache.clear();
   errandCache.clear();

   // Clear last winner
   lastWinner = NULL;
}

void GameBoard::executeSetup()
{
   // Place cards on the spots
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
   {
      Card::Ptr card = decks[itr->decks[round-1]]->draw();
      itr->setCard(card);
   }

   // Players must discard down at the end of setup for the first
   // round
   if (round == 1)
   {
      for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         unsigned int maxHandSize = itr->second.state->getMaxHandSize();
         unsigned int curHandSize = itr->second.state->getHandSize();
         if (curHandSize > maxHandSize)
         {
            Cards discarded = itr->second.player->discardCards(curHandSize-maxHandSize);
            unsigned int numDiscarded = 0;
            for (Cards::iterator cardItr = discarded.begin(); cardItr != discarded.end(); ++cardItr)
            {
               Card::Ptr card = (*cardItr);
               if (card != NULL)
               {
                  decks[card->deck]->discard(card);
                  numDiscarded++;
               }
            }
            if (numDiscarded != (curHandSize-maxHandSize))
            {
               throw std::runtime_error(itr->first +
                                        " failed to discard the appropriate number of cards.");
            }
         }
      }
   }
}

void GameBoard::executeEndOfRound()
{
   // Make sure the wildcards are discarded
   decks[CardDeck::WildCards]->reset();

   // Discard down to two
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      unsigned int maxHandSize = itr->second.state->getMaxHandSize();
      unsigned int curHandSize = itr->second.state->getHandSize();
      if (curHandSize > maxHandSize)
      {
         Cards discarded = itr->second.player->discardCards(curHandSize-maxHandSize);
         unsigned int numDiscarded = 0;
         for (Cards::iterator cardItr = discarded.begin(); cardItr != discarded.end(); ++cardItr)
         {
            Card::Ptr card = (*cardItr);
            if (card != NULL)
            {
               printf("%s discarded %s\n",itr->second.player->getName().c_str(),card->strName.c_str());
               decks[card->deck]->discard(card);
               numDiscarded++;
            }
         }
         if (numDiscarded != (curHandSize-maxHandSize))
         {
            throw std::runtime_error(itr->first +
                                     " failed to discard the appropriate number of cards.");
         }
      }
   }

   // Reset cards on player boards && depreciate properties
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      itr->second.state->board.executeEndOfRound();
   }

   // Release all board spots
   releasePlans();
   releasePropertyMarket();
   releaseSingleCardDraws();
   releaseBoardExtensions();
   releaseOperaSpot();
   releaseCardSpots();

   // Move round counter
   round++;

   // Move starting player marker
   firstPlayer++;
   if (firstPlayer >= players.size())
   {
      firstPlayer = 0;
   }

   // Clear Player Resources
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      Resources &resources = itr->second.state->resources;
      int money = resources[Resource::Money];
      resources.clear();
      resources[Resource::Money] = money;
      resources[Resource::BoardSpot] = itr->second.state->board.getNumAvailSpots();
      Cards handCards = itr->second.state->hand.getCards();
      for (size_t i = 0; i < handCards.size(); ++i)
      {
         switch (handCards[i]->type)
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
            default:
               break;
         }
      }
   }

   // Perform final calculation of player scores if
   // that was the final round
   if (getNumRoundsRemaining() == 0)
   {
      PlayerData *winner = &(players.begin()->second);
      for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         Resources &resources = itr->second.state->resources;
         itr->second.state->finalPropertyCount = 0;
         CardSpots spots = itr->second.state->board.getOccupiedSpots();
         for (int i = 0; i < spots.size(); ++i)
         {
            Card::Ptr card = spots.at(i)->getCard();
            if (card != NULL && card->group == CardGroup::Property)
            {
               // Need to add property value to end worth
               int value = card->valueLevels[card->curValue].value;
               value += propertyMarket.getPropertyAdjustment(card->name);
               value += 5; // Penalty for having property at end of game
               resources[Resource::Money] += value;
               itr->second.state->finalPropertyCount++;
            }
         }

         // Check if player beats out the winner
         if (winner == &(itr->second))
         {
            // Do nothing
         }
         else if (resources[Resource::Money] < winner->state->resources[Resource::Money])
         {
            winner = &(itr->second);
         }
         else if (resources[Resource::Money] == winner->state->resources[Resource::Money])
         {
            if (itr->second.state->finalPropertyCount == winner->state->finalPropertyCount)
            {
               if (itr->second.state->finalPlanPriority < winner->state->finalPlanPriority)
               {
                  winner = &(itr->second);
               }
            }
            else if (itr->second.state->finalPropertyCount < winner->state->finalPropertyCount)
            {
               winner = &(itr->second);
            }
         }
      }

      lastWinner = winner->player;
      gameStats.winner = lastWinner->getName();
      for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
      {
         if (itr->second.state->resources.find(Resource::Money) != itr->second.state->resources.end())
            gameStats.endingMoney[itr->first] = itr->second.state->resources[Resource::Money];
         else
            throw std::runtime_error("A player has ended the game without a money resource pool\n");
      }
   }
}

/*
 * Player Functions
 */
void GameBoard::initPlayers()
{
   Card::Ptr card = decks[CardDeck::LastWill]->draw();
   int startingMoney = card->valueLevels[0].value;
   gameStats.startingMoney = startingMoney;
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      itr->second.state->reset();
      itr->second.state->resources[Resource::Money] = startingMoney;
      itr->second.state->hand.addCard(decks[CardDeck::Properties]->draw());
      itr->second.state->hand.addCard(decks[CardDeck::Properties]->draw());
      itr->second.state->hand.addCard(decks[CardDeck::HelpersExpenses]->draw());
      itr->second.state->hand.addCard(decks[CardDeck::HelpersExpenses]->draw());
   }
}

/*
 * Plan Functions
 */
void GameBoard::initPlans()
{
   for (Plans::iterator itr = plans.begin(); itr != plans.end(); ++itr)
   {
      delete (*itr);
   }
   plans.clear();

   size_t numPlayers = getNumPlayers();
   if (numPlayers == 2 || numPlayers == 3)
   {
      plans.push_back(new Plan(1,5,2,1));
      plans.push_back(new Plan(2,2,2,2));
      plans.push_back(new Plan(3,0,2,3));
      plans.push_back(new Plan(4,7,1,2));
      plans.push_back(new Plan(5,1,1,4));
      plans.push_back(new Plan(6,2,2,3));
   }
   else if (numPlayers == 4 || numPlayers == 5)
   {
      plans.push_back(new Plan(1,1,2,1));
      plans.push_back(new Plan(2,1,1,2));
      plans.push_back(new Plan(3,6,2,1));
      plans.push_back(new Plan(4,3,2,2));
      plans.push_back(new Plan(5,0,2,3));
      plans.push_back(new Plan(6,6,2,2));
      plans.push_back(new Plan(7,0,1,4));
      plans.push_back(new Plan(8,2,2,3));
   }
}
void GameBoard::releasePlans()
{
   for (Plans::iterator itr = plans.begin(); itr != plans.end(); ++itr)
   {
      if ((*itr)->location != NULL)
      {
         (*itr)->location->releasePlan();
      }
   }
}

/*
 * Property Market Functions
 */
void GameBoard::initPropertyMarket()
{
   propertyMarket.reset();
   marketSpot.reset();
}
void GameBoard::releasePropertyMarket()
{
   marketSpot.executeEndOfRound();
}

/*
 * Single Card Draw Functions
 */
void GameBoard::initSingleCardDraws()
{
   for (map<string,BoardSpot>::iterator itr = singleCardDraws.begin(); itr != singleCardDraws.end(); ++itr)
   {
      itr->second.releaseCard();
   }
   singleCardDraws.clear();
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      singleCardDraws[itr->first] = BoardSpot(Spot::SingleDraw);
   }
}
void GameBoard::releaseSingleCardDraws()
{
   for (map<string,BoardSpot>::iterator itr = singleCardDraws.begin(); itr != singleCardDraws.end(); ++itr)
   {
      itr->second.executeEndOfRound();
   }
}

/*
 * Board Extension Functions
 */
void GameBoard::initBoardExtensions()
{
   for (BoardSpotObjects::iterator itr = boardExtensions.begin(); itr != boardExtensions.end();)
   {
      itr->releaseCard();
      itr = boardExtensions.erase(itr);
   }
   boardExtensions.push_back(BoardSpot(Spot::BoardExpansion));
   if (getNumPlayers() == 5)
   {
      boardExtensions.push_back(BoardSpot(Spot::BoardExpansion));
   }
   numBoardExtensions = 12;
}
void GameBoard::releaseBoardExtensions()
{
   for (BoardSpotObjects::iterator itr = boardExtensions.begin(); itr != boardExtensions.end(); ++itr)
   {
      itr->executeEndOfRound();
   }
}

/*
 * Opera Spot Functions
 */
void GameBoard::initOperaSpot()
{
   operaSpot.releaseCard();
   operaSpot = BoardSpot(Spot::Opera);
}
void GameBoard::releaseOperaSpot()
{
   operaSpot.executeEndOfRound();
}

/*
 * Card Spot Functions
 */
void GameBoard::initCardSpots()
{
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end();)
   {
      itr->releaseCard();
      itr = cardSpots.erase(itr);
   }

   size_t numPlayers = getNumPlayers();
   if (numPlayers == 2 || numPlayers == 3)
   {
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::Special));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::Special));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::Special,
                                    CardDeck::Special,
                                    CardDeck::Special,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::Properties,
                                    CardDeck::Properties,
                                    CardDeck::Properties,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::HelpersExpenses));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::WildCards));
      if (numPlayers == 3)
      {
         cardSpots.push_back(BoardSpot(Spot::Card,
                                       CardDeck::Properties,
                                       CardDeck::Properties,
                                       CardDeck::Events,
                                       CardDeck::Events,
                                       CardDeck::Events,
                                       CardDeck::Events,
                                       CardDeck::Events));
      }
   }
   else if (numPlayers == 4 || numPlayers == 5)
   {
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::Properties,
                                    CardDeck::Properties,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::Properties,
                                    CardDeck::Properties,
                                    CardDeck::Properties,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::HelpersExpenses,
                                    CardDeck::Events,
                                    CardDeck::Events,
                                    CardDeck::Events));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::HelpersExpenses));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::Special));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::Special));
      cardSpots.push_back(BoardSpot(Spot::Card,
                                    CardDeck::Special,
                                    CardDeck::Special,
                                    CardDeck::Special,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards,
                                    CardDeck::WildCards));
      cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::WildCards));
      if (numPlayers == 5)
      {
         cardSpots.push_back(BoardSpot(Spot::Card,CardDeck::Events));
      }
   }
}
void GameBoard::releaseCardSpots()
{
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
   {
      if (itr->card != NULL)
      {
         Decks::iterator deck = decks.find(itr->card->deck);
         if (deck != decks.end())
         {
            deck->second->discard(itr->card);
         }
         else
         {
            throw std::runtime_error(itr->card->name + " does not have a deck!");
         }
      }
      itr->executeEndOfRound();
   }
}

/*
 * Supporting Functions
 */
size_t GameBoard::getNumPlayers()
{
   size_t numPlayers = players.size();
   if (numPlayers >= 2 && numPlayers <= 5)
   {
      return numPlayers;
   }
   else
   {
      throw std::runtime_error("Unsupported number of players.");
   }
   return -1;
}

unsigned int GameBoard::getNumRoundsRemaining()
{
   unsigned int numRoundsLeft = 8 - round;
   for (PlayerStates::iterator itr = players.begin(); itr != players.end(); ++itr)
   {
      if (itr->second.state->resources[Resource::Money] <= 0)
      {
         numRoundsLeft = 0;
      }
   }
   return numRoundsLeft;
}

void GameBoard::addEventActionPermutations(GamePlayer::Ptr player,Card::Ptr card, Actions &outputActions)
{
   Actions actions;
   Action action(ActionType::Play,card);

   if (card->valueLevels.size() == 1)
   {
      // Handle base case
      if (card->valueLevels[0].resource == Resource::ActionAnd2Money)
      {
         action.cost[Resource::Action] = 1;
         action.cost[Resource::Money] = 2;
      }
      else
      {
         action.cost[card->valueLevels[0].resource] = card->valueLevels[0].value;
      }

      int baseMoneyExpense = 0;
      if (card->upgradeLevels.size() > 0)
      {
         baseMoneyExpense = card->upgradeLevels[0].expense;
      }
      action.cost[Resource::Money] = baseMoneyExpense;
      actions.push_back(action);

      // Handle upgrade levels
      if (card->upgradeLevels.size() == 2)
      {
         action.cost[card->upgradeLevels[1].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[1].expense;
         actions.push_back(action);
      }
      else if (card->upgradeLevels.size() == 3)
      {
         action.cost[card->upgradeLevels[1].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[1].expense;
         actions.push_back(action);
         action.cost[card->upgradeLevels[2].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[2].expense;
         actions.push_back(action);
         action.cost.clear();
         action.cost[card->valueLevels[0].resource] = card->valueLevels[0].value;
         action.cost[Resource::Money] = baseMoneyExpense;
         action.cost[card->upgradeLevels[2].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[2].expense;
         actions.push_back(action);
      }
      else if (card->upgradeLevels.size() == 4)
      {
         action.cost[card->upgradeLevels[1].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[1].expense;
         actions.push_back(action);
         action.cost[card->upgradeLevels[2].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[2].expense;
         actions.push_back(action);
         action.cost[card->upgradeLevels[3].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[3].expense;
         actions.push_back(action);
         action.cost.clear();
         action.cost[card->valueLevels[0].resource] = card->valueLevels[0].value;
         action.cost[Resource::Money] = baseMoneyExpense;
         action.cost[card->upgradeLevels[2].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[2].expense;
         actions.push_back(action);
         action.cost[card->upgradeLevels[3].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[3].expense;
         actions.push_back(action);
         action.cost.clear();
         action.cost[card->valueLevels[0].resource] = card->valueLevels[0].value;
         action.cost[Resource::Money] = baseMoneyExpense;
         action.cost[card->upgradeLevels[3].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[3].expense;
         actions.push_back(action);
         action.cost[card->upgradeLevels[1].cost] += 1;
         action.cost[Resource::Money] += card->upgradeLevels[1].expense;
         actions.push_back(action);
      }
   }
   else
   {
      // Party cards where you can spend a variable # of actions
      // for varying expense
      for (size_t i = 0; i < card->valueLevels.size(); ++i)
      {
         action.cost.clear();
         action.cost[card->valueLevels[i].resource] = card->valueLevels[i].value;
         action.cost[Resource::Money] = card->upgradeLevels[i].expense;
         actions.push_back(action);
      }
   }

   // Add permutations that arise from card effects
   Actions tmp = actions;
   Cards cards = players[player->getName()].state->board.getCards();
   for (size_t ndx = 0; ndx < tmp.size(); ++ndx)
   {
//      printf("ndx = %lu\n",ndx);
      Action action = tmp[ndx];
      vector<int> mods;
      for (size_t i = 0; i < cards.size(); ++i)
      {
         switch (cards[i]->effect)
         {
            case Effect::BoatTripAndPartySpend2Extra:
               if (action.card->type == CardType::BoatTrip ||
                   action.card->type == CardType::Party)
                  mods.push_back(2);
               break;
            case Effect::DinnerSpend1Extra:
               if (action.card->type == CardType::Dinner)
                  mods.push_back(1);
               break;
            case Effect::CarriageSpend1Extra:
               if (action.card->type == CardType::Carriage)
                  mods.push_back(1);
               break;
            case Effect::CarriageSpend2Extra:
               if (action.card->type == CardType::Carriage)
                  mods.push_back(2);
               break;
            case Effect::TheatreSpend3Extra:
               if (action.card->type == CardType::Theatre)
                  mods.push_back(3);
               break;
            default:
               break;
         }
      }

      if (mods.size() == 1)
      {
         action.cost[Resource::Money] += mods[0];
         actions.push_back(action);
      }
      else if (mods.size() == 2)
      {
         int baseCost = action.cost[Resource::Money];
         action.cost[Resource::Money] = baseCost + mods[0];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1];
         actions.push_back(action);
      }
      else if (mods.size() == 3)
      {
         int baseCost = action.cost[Resource::Money];
         action.cost[Resource::Money] = baseCost + mods[0];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1] + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1] + mods[2];
         actions.push_back(action);
      }
      else if (mods.size() > 3)
      {
         throw std::runtime_error(
            "GameBoard isn't handling all effect permutations on action.");
      }
   }

   // Transfer actions over to the output and convert "Action" resources
   // to "EventAction"
   for (size_t i = 0; i < actions.size(); ++i)
   {
      Resources::iterator itr = actions[i].cost.find(Resource::Action);
      if (itr != actions[i].cost.end())
      {
         actions[i].cost[Resource::EventAction] = itr->second;
         actions[i].cost.erase(itr);
      }
      outputActions.push_back(actions[i]);
   }
}

void GameBoard::addPropertyBuyPermutations(GamePlayer::Ptr player, Card::Ptr card, Actions &actions)
{
   Action action(ActionType::Play);
   action.card = card;
   action.cost[Resource::BoardSpot] = 1;

   // Add buy permutations that are independent of effects
   int marketAdjust = propertyMarket.getPropertyAdjustment(card->name);
   action.cost[card->valueLevels[0].resource] = card->valueLevels[0].value + marketAdjust;
   if (card->name == CardName::Farm)
      action.cost[Resource::BuySellFarmAction] = 1;
   else
      action.cost[Resource::BuySellNonFarmAction] = 1;
   actions.push_back(action);

   // Check for property price related effects
   Cards cards = players[player->getName()].state->board.getCards();
   for (size_t i = 0; i < cards.size(); ++i)
   {
      switch (cards[i]->effect)
      {
         case Effect::AdjustNonFarmValueBy2WhenBuySell:
            if (card->name != CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal + 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal - 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         case Effect::ReduceFarmValueBy2AndBuySell:
            if (card->name == CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal - 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         case Effect::ReduceNonFarmBy4WhenBuySell:
            if (card->name != CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal - 4;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         default:
            break;
      }
   }
}

void GameBoard::addPlayActionPermutations(GamePlayer::Ptr player, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   Cards handCards = state->hand.getCards();
   for (size_t i = 0; i < handCards.size(); ++i)
   {
      Action action(ActionType::Play);

      switch (handCards[i]->group)
      {
         case CardGroup::Event:
            addEventActionPermutations(player,handCards[i],actions);
            break;
         case CardGroup::HelperExpense:
            action.card = handCards[i];
            action.cost[Resource::BoardSpot] = 1;
            action.cost[handCards[i]->valueLevels[0].resource] = handCards[i]->valueLevels[0].value;
            actions.push_back(action);
            break;
         case CardGroup::Property:
            addPropertyBuyPermutations(player,handCards[i],actions);
            break;
         default:
            break;
      }
   }
}

void GameBoard::addUpgradePermutations(GamePlayer::Ptr player, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      Card::Ptr card = boardSpots[i]->getCard();
      unsigned int nextUpgrade = card->curLevel + 1;
      if (nextUpgrade < card->upgradeLevels.size())
      {
         Action action(ActionType::Upgrade);
         action.card = card;
         action.cost[Resource::Action] = 1;
//         action.cost[card->upgradeLevels[nextUpgrade].cost] =
//            card->upgradeLevels[nextUpgrade].expense;
         action.cost[card->upgradeLevels[nextUpgrade].cost] = 1;
         actions.push_back(action);
      }
   }
}

void GameBoard::addExpensePermutations(GamePlayer::Ptr player, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (!(boardSpots[i]->hasBeenExpensed()))
      {
         Card::Ptr card = boardSpots[i]->getCard();
         if (card->upgradeLevels.size() > 0 || card->type == CardType::TrainingGround)
         {
            if (card->group == CardGroup::HelperExpense)
               addHelperExpenseActions(player,card,actions);
            else if (card->group == CardGroup::Property)
               addPropertyExpenseActions(player,card,actions);
         }
      }
   }
}

void GameBoard::addHelperExpenseActions(GamePlayer::Ptr player, Card::Ptr card, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i <= card->curLevel; ++i)
   {
      // Add action for expensing without any effects
      Action action(ActionType::Expense);
      action.card = card;
      action.cost[Resource::Action] = card->expenseCost;
      int baseCost = 0;
      vector<int> baseCosts;
      if (card->type == CardType::TrainingGround)
      {
         switch (card->effect)
         {
            case Effect::Spend1PerFarmAndHorseOnFarm:
               for (size_t j = 0; j < boardSpots.size(); ++j)
               {
                  Card::Ptr spot = boardSpots[j]->getCard();
                  if (spot->type == CardType::Farm)
                  {
                     baseCost += 1;
                     for (size_t k = 0; k <= spot->curLevel; ++k)
                     {
                        if (spot->upgradeLevels[i].cost == Resource::Horse)
                           baseCost += 1;
                     }
                  }
               }
               break;
            case Effect::Spend2PerHorseOnFarm:
               for (size_t j = 0; j < boardSpots.size(); ++j)
               {
                  Card::Ptr spot = boardSpots[j]->getCard();
                  if (spot->type == CardType::Farm)
                  {
                     for (size_t k = 0; k <= spot->curLevel; ++k)
                     {
                        if (spot->upgradeLevels[i].cost == Resource::Horse)
                           baseCost += 2;
                     }
                  }
               }
               break;
            case Effect::Spend2PerFarm:
               for (size_t j = 0; j < boardSpots.size(); ++j)
               {
                  Card::Ptr spot = boardSpots[j]->getCard();
                  if (spot->type == CardType::Farm)
                  {
                     baseCost += 2;
                  }
               }
               break;
            case Effect::Spend1PerFarmAndDogOnFarm:
               for (size_t j = 0; j < boardSpots.size(); ++j)
               {
                  Card::Ptr spot = boardSpots[j]->getCard();
                  if (spot->type == CardType::Farm)
                  {
                     baseCost += 1;
                     for (size_t k = 0; k <= spot->curLevel; ++k)
                     {
                        if (spot->upgradeLevels[i].cost == Resource::Dog)
                           baseCost += 1;
                     }
                  }
               }
               break;
            case Effect::Spend2PerDogOnFarm:
               for (size_t j = 0; j < boardSpots.size(); ++j)
               {
                  Card::Ptr spot = boardSpots[j]->getCard();
                  if (spot->type == CardType::Farm)
                  {
                     for (size_t k = 0; k <= spot->curLevel; ++k)
                     {
                        if (spot->upgradeLevels[i].cost == Resource::Dog)
                           baseCost += 2;
                     }
                  }
               }
               break;
            default:
               throw std::runtime_error(
                  "A Training Ground card doesn't have an effect.");
               break;
         }
      }
      else
      {
         baseCost = card->upgradeLevels[i].expense;
      }
      action.cost[Resource::Money] = baseCost;
      actions.push_back(action);

      // Add actions for expensing with effects
      vector<int> mods;
      printf("mods.size() = %lu\n",mods.size());
      for (size_t j = 0; j < boardSpots.size(); ++j)
      {
         Card::Ptr spot = boardSpots[j]->getCard();
         switch (spot->effect)
         {
            case Effect::BoatTripAndPartySpend2Extra:
               if (card->type == CardType::BoatTrip || card->type == CardType::Party)
               {
                  printf("Adding mod for effect \"%s\"\n",Effect::toString(spot->effect).c_str());
                  mods.push_back(2);
               }
               break;
            case Effect::DinnerSpend1Extra:
               if (card->type == CardType::Dinner)
               {
                  printf("Adding mod for effect \"%s\"\n",Effect::toString(spot->effect).c_str());
                  printf("spot card id = \"%s\"\n",CardId::toString(spot->id).c_str());
                  mods.push_back(1);
               }
               break;
            case Effect::CarriageSpend1Extra:
               if (card->type == CardType::Carriage)
               {
                  printf("Adding mod for effect \"%s\"\n",Effect::toString(spot->effect).c_str());
                  mods.push_back(1);
               }
               break;
            case Effect::CarriageSpend2Extra:
               if (card->type == CardType::Carriage)
               {
                  printf("Adding mod for effect \"%s\"\n",Effect::toString(spot->effect).c_str());
                  mods.push_back(2);
               }
               break;
            case Effect::TheatreSpend3Extra:
               if (card->type == CardType::Theatre)
               {
                  printf("Adding mod for effect \"%s\"\n",Effect::toString(spot->effect).c_str());
                  mods.push_back(3);
               }
               break;
            default:
               break;
         }
      }

      // Generate actions based on the mods
      if (mods.size() == 1)
      {
         action.cost[Resource::Money] = baseCost + mods[0];
         actions.push_back(action);
      }
      else if (mods.size() == 2)
      {
         action.cost[Resource::Money] = baseCost + mods[0];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1];
         actions.push_back(action);
      }
      else if (mods.size() == 3)
      {
         action.cost[Resource::Money] = baseCost + mods[0];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[1] + mods[2];
         actions.push_back(action);
         action.cost[Resource::Money] = baseCost + mods[0] + mods[1] + mods[2];
         actions.push_back(action);
      }
      else if (mods.size() > 3)
      {
         throw std::runtime_error(
            "More than 3 helper expense mods that need to be handled.");
      }
   }
}

void GameBoard::addPropertyExpenseActions(GamePlayer::Ptr player, Card::Ptr card, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   // Different actions for the individual upgrade levels
   for (size_t i = 0; i <= card->curLevel; ++i)
   {
      Action action(ActionType::Expense);
      action.card = card;
      int baseExpense = card->upgradeLevels[i].expense;
      if (card->type == CardType::Farm)
         action.cost[Resource::FarmUpkeepAction] = card->expenseCost;
      else
         action.cost[Resource::NonFarmUpkeepAction] = card->expenseCost;
      action.cost[Resource::Money] = baseExpense;
      actions.push_back(action);

      // Get mods by relevant effects
      vector<int> mods;
      for (size_t j = 0; j < boardSpots.size(); ++j)
      {
         Card::Ptr spot = boardSpots[j]->getCard();
         switch (spot->effect)
         {
            case Effect::TownAndManorUpkeep3Extra:
               if (card->name == CardName::TownHouse ||
                   card->name == CardName::ManorHouse)
               {
                  mods.push_back(3);
               }
               break;
            case Effect::MansionAndManorUpkeep3Extra:
               if (card->name == CardName::Mansion ||
                   card->name == CardName::ManorHouse)
               {
                  mods.push_back(3);
               }
               break;
            case Effect::MansionAndTownUpkeep3Extra:
               if (card->name == CardName::Mansion ||
                   card->name == CardName::TownHouse)
               {
                  mods.push_back(3);
               }
               break;
            default:
               break;
         }
      }

      // Apply mods to the action
      if (mods.size() == 1)
      {
         action.cost[Resource::Money] = baseExpense + mods[0];
         actions.push_back(action);
      }
      else if (mods.size() == 2)
      {
         action.cost[Resource::Money] = baseExpense + mods[0];
         actions.push_back(action);
         action.cost[Resource::Money] = baseExpense + mods[1];
         actions.push_back(action);
         action.cost[Resource::Money] = baseExpense + mods[0] + mods[1];
         actions.push_back(action);
      }
      else if (mods.size() > 2)
      {
         throw std::runtime_error(
            "More than 2 property expense mods need to be handled.");
      }
   }
}

void GameBoard::addActivatePermutations(GamePlayer::Ptr player, Actions &actions)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      Card::Ptr card = boardSpots[i]->getCard();
      if (!boardSpots[i]->hasBeenActivated() &&
          card != NULL &&
          card->effect != Effect::None)
      {
         Effects effects = Effect::getEffectsWithLimitedActivation();
         for (size_t j = 0; j < effects.size(); ++j)
         {
            if (card->effect == effects[j])
            {
               Action action(ActionType::Activate);
               action.card = card;
               actions.push_back(action);
               break;
            }
         }
      }
   }
}

void GameBoard::addPropertySellPermutations(GamePlayer::Ptr player, Card::Ptr card, Actions &outActions)
{
   Actions actions;
   Action action(ActionType::Sell);
   action.card = card;

   // Add sell permutations that are independent of effects
   int marketAdjust = propertyMarket.getPropertyAdjustment(card->name);
   unsigned int valNdx = card->curValue;
   action.cost[card->valueLevels[valNdx].resource] = card->valueLevels[valNdx].value + marketAdjust;
   if (card->name == CardName::Farm)
      action.cost[Resource::BuySellFarmAction] = 1;
   else
      action.cost[Resource::BuySellNonFarmAction] = 1;
   actions.push_back(action);

   // Check for property price related effects
   Cards cards = players[player->getName()].state->board.getCards();
   for (size_t i = 0; i < cards.size(); ++i)
   {
      switch (cards[i]->effect)
      {
         case Effect::AdjustNonFarmValueBy2WhenBuySell:
            if (card->name != CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal + 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal - 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         case Effect::ReduceFarmValueBy2AndBuySell:
            if (card->name == CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal - 2;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         case Effect::ReduceNonFarmBy4WhenBuySell:
            if (card->name != CardName::Farm)
            {
               int origVal = action.cost[Resource::Money];
               action.cost[Resource::Money] = origVal - 4;
               actions.push_back(action);
               action.cost[Resource::Money] = origVal;
            }
            break;
         default:
            break;
      }
   }

   // Since we're selling the money flows back to the player,
   // so we need to flip the sign for the money resource
   for (size_t i = 0; i < actions.size(); ++i)
   {
      actions[i].cost[Resource::Money] *= -1;
      outActions.push_back(actions[i]);
   }
}

bool GameBoard::playerCanPay(GamePlayer::Ptr player, Action action)
{
   bool canPay = true;

   // Only check if there is actually a cost
   if (action.cost.size() > 0)
   {
      // Check money
      Resources::iterator money = action.cost.find(Resource::Money);
      if (money != action.cost.end())
      {
         if (!players[player->getName()].state->canSpendMoney(money->second))
         {
            return false;
         }
      }

      // work on a copy of the player's resources
      Resources source = players[player->getName()].state->resources;

      // Check boardspace
      Resources::iterator itr = action.cost.find(Resource::BoardSpot);
      if (itr != action.cost.end())
      {
         if (source[itr->first] < itr->second)
         {
            return false;
         }
      }

      // Check action sub-types
      itr = action.cost.find(Resource::EventAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::FarmUpkeepAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::NonFarmUpkeepAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::SellNonFarmAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::BuySellFarmAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::BuySellNonFarmAction);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;

      // Check general actions
      itr = action.cost.find(Resource::Action);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;

      // Check companions
      itr = action.cost.find(Resource::Horse);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::Dog);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::Chef);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
      itr = action.cost.find(Resource::Guest);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;

      // Wildcard check shouldn't matter but I'll do it anyway
      itr = action.cost.find(Resource::WildCard);
      if (itr != action.cost.end() &&
         !subtrackResource(itr->first,itr->second,source))
         return false;
   }

   return canPay;
}

bool GameBoard::subtrackResource(ResourceType resource, int qty, Resources &pool)
{
   bool pass = true;
   switch (resource)
   {
      case Resource::EventAction:
      case Resource::FarmUpkeepAction:
      case Resource::NonFarmUpkeepAction:
      case Resource::BuySellFarmAction:
      case Resource::BuySellNonFarmAction:
      case Resource::SellNonFarmAction:
      case Resource::Action:
         if (pool[resource] >= qty)
         {
            pool[resource] -= qty;
         }
         else
         {
            qty -= pool[resource];
            pool[resource] = 0;
            pool[Resource::Action] -= qty;
            if (pool[Resource::Action] < 0)
               pass = false;
         }
         break;
      case Resource::Horse:
      case Resource::Dog:
      case Resource::Chef:
      case Resource::Guest:
      case Resource::WildCard:
         if (pool[resource] >= qty)
         {
            pool[resource] -= qty;
         }
         else
         {
            qty -= pool[resource];
            pool[resource] = 0;
            pool[Resource::WildCard] -= qty;
            if (pool[Resource::WildCard] < 0)
               pass = false;
         }
         break;
      case Resource::Money:
      case Resource::BoardSpot:
         pool[resource] -= qty;
         break;
      default:
         throw std::runtime_error("Attempting to subtract an invalid resource.");
         break;
   }

   return pass;
}

void GameBoard::print()
{
   printf("Market State:\n");
   PropertyAdjustments adjustments = propertyMarket.getPropertyAdjustments();
   for (PropertyAdjustments::iterator itr = adjustments.begin(); itr != adjustments.end(); ++itr)
   {
      switch (itr->first)
      {
         case CardName::Farm:
            printf("Farm");
            break;
         case CardName::TownHouse:
            printf("TownHouse");
            break;
         case CardName::ManorHouse:
            printf("ManorHouse");
            break;
         case CardName::Mansion:
            printf("Mansion");
            break;
         default:
            printf("INVALID");
            break;
      }
      printf(" : %d\n",itr->second);
   }
   printf("Card Spots:\n");
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
   {
      printf("Card:: ");
      if (itr->card != NULL)
      {
         printf("%s\n",itr->card->strName.c_str());
      }
      else
      {
         printf("NULL\n");
      }
   }
}

/*
 * Process Action Functions
 */
bool GameBoard::processPlayAction(GamePlayer::Ptr player, Action action)
{
   printf("Gameboard is processing a play action\n");
   // Make sure the player is attempting to play a card they control
   PlayerState::Ptr state = players[player->getName()].state;
   Cards handCards = state->hand.getCards();
   Card::Ptr card = NULL;
   for (size_t i = 0; i < handCards.size(); ++i)
   {
      if (handCards[i] == action.card)
      {
         card = action.card;
         break;
      }
   }
   if (card == NULL)
      return false;

   // Make sure the play action is valid
   printf("   generating possible actions by player\n");
   Actions actions;
   addPlayActionPermutations(player,actions);

   printf("   checking for matching action\n");
   // Find the matching action
   bool matchFound = false;
   for (size_t i = 0; i < actions.size(); ++i)
   {
      if (actions[i].cost == action.cost)
      {
         matchFound = true;
         break;
      }
   }
   if (!matchFound)
      return false;

   printf("   executing the play\n");
   // Execute the play
   bool result = false;
   if (playerCanPay(player,action))
   {
      result = true;

      // Subtrack the resources to pay for the play
      Resources resources = state->resources;
      for (Resources::iterator itr = action.cost.begin(); itr != action.cost.end(); ++itr)
      {
         result &= subtrackResource(itr->first,itr->second,resources);
      }

      if (result)
      {
         // Set player's new resource set
         state->resources = resources;

         // Process any effects if the card was an event
         if (card->group == CardGroup::Event)
         {
            switch (card->effect)
            {
               case Effect::PlaceDogOrHorseOnFarmTwice:
                  processBreedersFair(player);
                  break;
               case Effect::DepreciateAHouseValue3Times:
                  processWildParty(player);
                  break;
               case Effect::Gain2Actions:
                  state->resources[Resource::Action] += 2;
                  break;
               case Effect::Gain3Actions:
                  state->resources[Resource::Action] += 3;
                  break;
               default:
                  break;
            }
         }

         // Discard the card if it is an event, otherwise put
         // it on the player's board
         if (card->group == CardGroup::Event)
         {
            card->reset();
            decks[card->deck]->discard(card);
         }
         else
         {
            state->board.placeCard(card);
         }
      }
      else
      {
         throw std::runtime_error(
            "Failed to subtrack resources that should've already been verified.");
      }
   }

   return result;
}

bool GameBoard::processUpgradeAction(GamePlayer::Ptr player, Action action)
{
   // Make sure the player is attempting to activate a card they control
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   CardSpot::Ptr spot = NULL;
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (boardSpots[i]->getCard() == action.card)
      {
         spot = boardSpots[i];
         break;
      }
   }
   if (spot == NULL)
   {
      throw std::runtime_error("Failed to find board spot with desired card to upgrade.");
      return false;
   }

   // Make sure the upgrade action is valid
   Actions actions;
   addUpgradePermutations(player,actions);

   // Find the matching action
   bool matchFound = false;
   for (size_t i = 0; i < actions.size(); ++i)
   {
      if (actions[i].cost == action.cost)
      {
         matchFound = true;
         break;
      }
   }
   if (!matchFound)
   {
      throw std::runtime_error("Failed to find matching upgrade permutation.");
      return false;
   }

   // Execute the expense
   bool result = false;
   if (playerCanPay(player,action))
   {
      result = true;
      Resources resources = state->resources;
      for (Resources::iterator itr = action.cost.begin(); itr != action.cost.end(); ++itr)
      {
         result &= subtrackResource(itr->first,itr->second,resources);
      }

      if (result)
      {
         // Set player's new resource set
         state->resources = resources;

         // Upgrade the card
         action.card->curLevel += 1;
      }
      else
      {
         throw std::runtime_error(
            "Failed to subtrack resources that should've already been verified.");
      }
   }

   return result;
}

bool GameBoard::processExpenseAction(GamePlayer::Ptr player, Action action)
{
   // Make sure the player is attempting to activate a card they control
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   CardSpot::Ptr spot = NULL;
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (boardSpots[i]->getCard() == action.card)
      {
         if (!(boardSpots[i]->hasBeenExpensed()))
            spot = boardSpots[i];
         break;
      }
   }
   if (spot == NULL)
   {
      throw std::runtime_error("Failed to find board spot with desired card to expense.");
      return false;
   }

   // Make sure the expense action is valid
   Actions actions;
   addExpensePermutations(player,actions);

   // Find the matching action
   bool matchFound = false;
   for (size_t i = 0; i < actions.size(); ++i)
   {
      if (actions[i].cost == action.cost)
      {
         matchFound = true;
         break;
      }
   }
   if (!matchFound)
   {
      throw std::runtime_error("Failed to find matching expense permutation.");
      return false;
   }

   // Execute the expense
   bool result = false;
   if (playerCanPay(player,action))
   {
      result = true;
      Resources resources = state->resources;
      for (Resources::iterator itr = action.cost.begin(); itr != action.cost.end(); ++itr)
      {
         result &= subtrackResource(itr->first,itr->second,resources);
      }

      if (result)
      {
         // Set player's new resource set
         state->resources = resources;

         // Mark the spot as expensed
         spot->expense();
      }
      else
      {
         throw std::runtime_error(
            "Failed to subtrack resources that should've already been verified.");
      }
   }

   return result;
}

bool GameBoard::processActivateAction(GamePlayer::Ptr player, Action action)
{
   // Make sure the player is attempting to activate a card they control
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   CardSpot::Ptr spot = NULL;
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (boardSpots[i]->getCard() == action.card)
      {
         if (!(boardSpots[i]->hasBeenActivated()))
            spot = boardSpots[i];
         break;
      }
   }
   if (spot == NULL)
      return false;

   // Possible activate actions for the player
   Actions actions;
   addActivatePermutations(player,actions);

   // Find the matching action
   bool matchFound = false;
   for (size_t i = 0; i < actions.size(); ++i)
   {
      if (actions[i].cost == action.cost)
      {
         matchFound = true;
         break;
      }
   }
   if (!matchFound)
      return false;

   // Execute the activate
   bool result = true;
   switch (action.card->effect)
   {
      case Effect::Gain1EventAction:
         state->resources[Resource::EventAction] += 1;
         break;
      case Effect::Gain1Action:
         state->resources[Resource::Action] += 1;
         break;
      case Effect::Gain2Actions:
         state->resources[Resource::Action] += 2;
         break;
      case Effect::Gain3Actions:
         state->resources[Resource::Action] += 3;
         break;
      case Effect::BuySell2NonFarms:
         state->resources[Resource::BuySellNonFarmAction] += 2;
         break;
      case Effect::Perform2FarmUpkeep:
         state->resources[Resource::FarmUpkeepAction] += 2;
         break;
      case Effect::ReduceFarmValueBy2AndBuySell:
         state->resources[Resource::BuySellFarmAction] += 1;
         break;
      case Effect::Perform1NonFarmUpkeep:
         state->resources[Resource::NonFarmUpkeepAction] += 1;
         break;
      case Effect::Perform1FarmUpkeep:
         state->resources[Resource::FarmUpkeepAction] += 1;
         break;
      default:
         break;
   }
   spot->activate();

   return result;
}

bool GameBoard::processSellAction(GamePlayer::Ptr player, Action action)
{
   // Make sure the player is attempting to sell a property they control
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   CardSpot::Ptr spot = NULL;
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (boardSpots[i]->getCard() == action.card)
      {
         spot = boardSpots[i];
         break;
      }
   }
   if (spot == NULL)
      return false;

   // Possible sell actions for the player
   Actions actions;
   addPropertySellPermutations(player,action.card,actions);

   // Find the matching action
   bool matchFound = false;
   for (size_t i = 0; i < actions.size(); ++i)
   {
      if (actions[i].cost == action.cost)
      {
         matchFound = true;
         break;
      }
   }
   if (!matchFound)
      return false;

   // Execute the sell
   bool result = false;
   if (playerCanPay(player,action))
   {
      result = true;
      Resources resources = state->resources;
      for (Resources::iterator itr = action.cost.begin(); itr != action.cost.end(); ++itr)
      {
         result &= subtrackResource(itr->first,itr->second,resources);
      }


      if (result)
      {
         // Set player's new resource set
         state->resources = resources;

         // Reset and discard the card
         action.card->reset();
         decks[action.card->deck]->discard(action.card);
      }
      else
      {
         throw std::runtime_error(
            "Failed to subtrack resources that should've already been verified.");
      }
   }

   return result;
}

bool GameBoard::processDiscardAction(GamePlayer::Ptr player, Action action)
{
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   bool result = false;

   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      if (boardSpots[i]->getCard() == action.card)
      {
         // Discard the card to it's appropriate deck
         action.card->reset();
         decks[action.card->deck]->discard(action.card);
         state->resources[Resource::BoardSpot] += 1;
         result = true;
         break;
      }
   }

   return result;
}

void GameBoard::processBreedersFair(GamePlayer::Ptr player)
{
   // Get the cards the player wants to upgrade
   Cards tgts = player->placeTwoDogsHorses();
   Cards upgrades;

   // make sure the player controls those cards
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      for (size_t j = 0; j < tgts.size(); ++j)
      {
         if (boardSpots[i]->getCard() == tgts[j])
         {
            upgrades.push_back(tgts[j]);
         }
      }
   }

   // Process the valid upgrades
   if (upgrades.size() <= 2)
   {
      for (size_t i = 0; i < upgrades.size(); ++i)
      {
         unsigned int nextUpgrade = upgrades[i]->curLevel + 1;
         if (nextUpgrade < upgrades[i]->upgradeLevels.size() &&
             upgrades[i]->type == CardType::Farm &&
             (upgrades[i]->upgradeLevels[nextUpgrade].cost == Resource::Dog ||
              upgrades[i]->upgradeLevels[nextUpgrade].cost == Resource::Horse))
         {
            ResourceType cost = upgrades[i]->upgradeLevels[nextUpgrade].cost;
            Resources source = state->resources;
            if (subtrackResource(cost,1,source))
            {
               state->resources = source;
               upgrades[i]->curLevel = nextUpgrade;
            }
         }
      }
   }
   else
   {
      throw std::runtime_error(
         "Player tried use the PlaceDogHorseTwice effect in an illegal way.");
   }
}

void GameBoard::processWildParty(GamePlayer::Ptr player)
{
   // Get the cards the player wants to upgrade
   Cards choices = player->depreciateThreeProperties();
   Cards tgts;

   // make sure the player controls those cards
   PlayerState::Ptr state = players[player->getName()].state;
   CardSpots boardSpots = state->board.getOccupiedSpots();
   for (size_t i = 0; i < boardSpots.size(); ++i)
   {
      for (size_t j = 0; j < choices.size(); ++j)
      {
         if (boardSpots[i]->getCard() == choices[j])
         {
            tgts.push_back(choices[j]);
         }
      }
   }

   // Process the valid upgrades
   if (tgts.size() <= 3)
   {
      for (size_t i = 0; i < tgts.size(); ++i)
      {
         if (tgts[i]->group == CardGroup::Property)
         {
            tgts[i]->depreciate();
         }
      }
   }
   else
   {
      throw std::runtime_error(
         "Player tried use the WildParty effect in an illegal way.");
   }
}

void GameBoard::printState()
{
   printf("GameBoard State\n");
   printf("Round: %u\n",round);

   // Plans
   printf("Plans (Player, Priority, Cards, Errands, Actions):\n");
   for (Plans::iterator itr = plans.begin(); itr != plans.end(); ++itr)
   {
      printf("   \"%s\", %d, %d, %d, %d\n",
             (*itr)->getOwnersName().c_str(),
             (*itr)->priority,
             (*itr)->cards,
             (*itr)->errands,
             (*itr)->actions);
   }

   // Property Market
   printf("Property Market:\n");
   printf("   Player: \"%s\"\n",marketSpot.player.c_str());
   printf("   Farm Adjust       : %d\n",propertyMarket.getPropertyAdjustment(CardName::Farm));
   printf("   Town House Adjust : %d\n",propertyMarket.getPropertyAdjustment(CardName::TownHouse));
   printf("   Manor House Adjust: %d\n",propertyMarket.getPropertyAdjustment(CardName::ManorHouse));
   printf("   Mansion Adjust    : %d\n",propertyMarket.getPropertyAdjustment(CardName::Mansion));

   // Single Card Draws
   printf("Single Card Draws:\n");
   for (map<string,BoardSpot>::iterator itr = singleCardDraws.begin(); itr != singleCardDraws.end(); ++itr)
   {
      printf("   \"%s\", ",itr->first.c_str());
      if (itr->second.occupied)
         printf("Occupied\n");
      else
         printf("Empty\n");
   }

   // Board Extensions
   printf("Board Extensions:\n");
   int count = 0;
   for (BoardSpotObjects::iterator itr = boardExtensions.begin(); itr != boardExtensions.end(); ++itr)
   {
      printf("   %d) %d, \"%s\"\n",++count,itr->occupied,itr->player.c_str());
   }

   // Opera Spot
   printf("Opera (Occupied, Player):\n");
   printf("   %d, \"%s\"\n",operaSpot.occupied,operaSpot.player.c_str());

   // Card Spots
   printf("Card Spots:\n");
   count = 0;
   for (BoardSpotObjects::iterator itr = cardSpots.begin(); itr != cardSpots.end(); ++itr)
   {
      bool occupied = itr->occupied;
      string player = itr->player;
      Card::Ptr card = itr->card;
      printf("   %d) %d, \"%s\", ",++count, occupied, player.c_str());
      if (card != NULL)
         printf("%s\n",CardId::toString(card->id).c_str());
      else
         printf("empty\n");
   }
}
