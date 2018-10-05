/*
 * GamePlayer.h
 *
 *  Created on: Sep 15, 2018
 *      Author: milites
 */

#ifndef GAMEPLAYER_H_
#define GAMEPLAYER_H_

#include <assets/cards/Card.h>
#include <vector>

class GamePlayer
{
public:
   typedef GamePlayer* Ptr;

   virtual ~GamePlayer();

   virtual std::string getName()=0;
   virtual Cards discardCards(unsigned int numToDiscard)=0;
   virtual Cards depreciateThreeProperties()=0;
   virtual Cards placeTwoDogsHorses()=0;

protected:
   GamePlayer();
};

#endif /* GAMEPLAYER_H_ */
