/*
 * main.cpp
 *
 *  Created on: Sep 3, 2018
 *      Author: milites
 */

#include <stdio.h>
#include <GameMaster.h>

int main(int argc, char **argv)
{
   printf("starting\n");
   GameMaster gm;

   gm.run(argc,argv);

   printf("finished\n");
   return 0;
}
