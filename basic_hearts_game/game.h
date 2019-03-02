#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"

#define MAX_GAME_POINTS 100

typedef struct Game Game;

Game *GameCreate(char *_name);
void GameDestroy(Game *_game);
ADTErr RunGame(Game *_game);



#endif /* GAME_H_ */
