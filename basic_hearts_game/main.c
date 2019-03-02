#include <stdio.h>
#include "game.h"
#include "shapesandcolors.h"

#define HUMAN_PLAYER_MAX_NAME_LEN 50

int main()
{
    Game *game;
    char name[HUMAN_PLAYER_MAX_NAME_LEN];
    printf("%sWhats your name?%s\n", YELLOW, NORMAL);
    scanf("%s", name);
    game = GameCreate(name);
   	RunGame(game);
    GameDestroy(game);
    return 0;
}
