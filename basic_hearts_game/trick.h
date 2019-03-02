#ifndef TRICK_H_
#define TRICK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"
#include "player.h"

#define NUM_OF_PLAYERS 4

typedef struct Trick Trick;

Trick *TrickCreate(Player **_players, int _starter);
void TrickDestroy(Trick *_trick);
void TrickPlayAllPlayers(Trick *_trick);
int TrickTakerIndex(Trick *_trick);
int TrickGetPoints(Trick *_trick);
	
void TrickPrintTable(Trick *_trick);
 
#endif /* TRICK_H_ */
