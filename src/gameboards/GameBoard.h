/*
 * GameBoard.h
 *
 *  Created on: Sep 8, 2018
 *      Author: milites
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

#include <gameboards/GameBoardInterface.h>
#include <gameboards/GamePlayer.h>
#include <assets/Action.h>
#include <assets/BoardSpot.h>
#include <assets/Deck.h>
#include <assets/PlayerState.h>
#include <assets/Plan.h>
#include <assets/PropertyMarket.h>
#include <map>

struct PlayerData
{
   PlayerData() : player(NULL), state(NULL) {};
   GamePlayer::Ptr player;
   PlayerState::Ptr state;
};

struct ActionData
{
   GamePlayer::Ptr player;
   int round;
   Action action;
};

class GameBoard : public GameBoardInterface
{
public:
   typedef GameBoard* Ptr;

   GameBoard();
   virtual ~GameBoard();

   /*
    * Game Board Interface Functions
    */
   virtual Plans getAvailablePlans();
   virtual Plans getAllPlans();
   virtual BoardSpots getAvailableErrands(GamePlayer::Ptr);
   virtual BoardSpots getAllErrands(GamePlayer::Ptr);
   virtual Actions getAvailableActions(GamePlayer::Ptr);
   virtual bool processAction(GamePlayer::Ptr,Action);

   /*
    * Game Master Functions
    */
   virtual PlayerState::Ptr registerPlayer(GamePlayer::Ptr);
   virtual void initialize();
   virtual void executeSetup();
   virtual void executeEndOfRound();

   void print();

   typedef std::map<CardDeckType,Deck::Ptr> Decks;
   typedef std::pair<CardDeckType,Deck::Ptr> DeckEntry;
   Decks decks;

   typedef std::map<std::string,PlayerData> PlayerStates;
   typedef std::pair<std::string,PlayerData> PlayerStateEntry;
   PlayerStates players;
   unsigned int firstPlayer;
   void initPlayers();

   // Round Tracker
   unsigned int round; // Current round

   // Daily Plans
   Plans plans;
   void initPlans();
   void releasePlans();

   // Property Market
   PropertyMarket propertyMarket;
   BoardSpot marketSpot;
   void initPropertyMarket();
   void releasePropertyMarket();

   // Single Card Draws
   std::map<std::string,BoardSpot> singleCardDraws;
   void initSingleCardDraws();
   void releaseSingleCardDraws();

   // Board Extension
   BoardSpotObjects boardExtensions;
   unsigned int numBoardExtensions; // Number of remaining board extensions
   void initBoardExtensions();
   void releaseBoardExtensions();

   // Opera
   BoardSpot operaSpot;
   void initOperaSpot();
   void releaseOperaSpot();

   // Card Spots
   // HEY! LISTEN! The gameboard will have to take care for
   // 2-3 player games where 3 spots share 2 helpers! This
   // applies to indexes 0-2!
   BoardSpotObjects cardSpots;
   void initCardSpots();
   void releaseCardSpots();

   // Supporting Functions
   size_t getNumPlayers();
   unsigned int getNumRoundsRemaining();
   void addPlayActionPermutations(GamePlayer::Ptr,Actions&);
   void addEventActionPermutations(GamePlayer::Ptr,Card::Ptr,Actions&);
   void addPropertyBuyPermutations(GamePlayer::Ptr,Card::Ptr,Actions&);
   void addActivatePermutations(GamePlayer::Ptr,Actions&);
   void addUpgradePermutations(GamePlayer::Ptr,Actions&);
   void addExpensePermutations(GamePlayer::Ptr,Actions&);
   void addHelperExpenseActions(GamePlayer::Ptr,Card::Ptr,Actions&);
   void addPropertyExpenseActions(GamePlayer::Ptr,Card::Ptr,Actions&);
   void addPropertySellPermutations(GamePlayer::Ptr,Card::Ptr,Actions&);
   bool playerCanPay(GamePlayer::Ptr,Action);
   bool subtrackResource(ResourceType,int,Resources &);

   // Process Action Functions
   bool processPlayAction(GamePlayer::Ptr,Action);
   bool processUpgradeAction(GamePlayer::Ptr,Action);
   bool processExpenseAction(GamePlayer::Ptr,Action);
   bool processActivateAction(GamePlayer::Ptr,Action);
   bool processSellAction(GamePlayer::Ptr,Action);
   bool processDiscardAction(GamePlayer::Ptr,Action);
   void processBreedersFair(GamePlayer::Ptr);
   void processWildParty(GamePlayer::Ptr);

   // Print Functions
   void printState();

   // Logging Data
   std::vector<ActionData> actionCache;
   GamePlayer::Ptr lastWinner;
};

#endif /* GAMEBOARD_H_ */
