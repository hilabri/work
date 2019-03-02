#ifndef ROUND_H_
#define ROUND_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"
#include "player.h"

typedef enum
{
	SWITCH_NONE = 0,	
	SWITCH_RIGHT,
	SWITCH_ACCROSS,
	SWITCH_LEFT,
	NUM_OF_SWITCH
}Switch;

typedef struct Round Round;

Round *RoundCreate(Player **_players, Switch _switch);
void RoundDestroy(Round *_round);
ADTErr RoundPlay(Round *_round);




#endif /* ROUND_H_ */
