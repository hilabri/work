#include <stdio.h>
#include <time.h>
#include "ADTDefs.h"
#include "round.h"
#include "table.h"
#include "card.h"
#include "deck.h"
#include "trick.h"
#include "game.h"
#include "player.h"
#include "shapesandcolors.h"

#define MAGIC 0xBEEFBEEF 
#define IS_VALID(A)             ((A) && (MAGIC == (A)->m_magic)) 
 
#define FALSE 0
#define TRUE 1

struct Game
{
	Round *m_round;
	int m_numOfRounds;
	Player *m_players[NUM_OF_PLAYERS];
    int m_magic;
};

/*****************************************/

Game *GameCreate(char *_name)
{
    int i;
    char *computerNames[] = {"Billa", "Danna", "Maxa"};
    Game *game = malloc(sizeof(Game));
    if(!game)
    {
        return NULL;
    }
  	srand(time(0));
    game->m_numOfRounds = 0;
    game->m_players[0] = PlayerCreate(_name, HUMAN);
    for (i = 1; i < NUM_OF_PLAYERS; i++)
    {
        game->m_players[i] = PlayerCreate(computerNames[i-1], COMPUTER);
    }
    game->m_magic = MAGIC;
    return game;
} 

/*****************************************/
void GameDestroy(Game *_game)
{
    int i;
    if(!IS_VALID(_game))
    {   
        return;
    }
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        PlayerDestroy(_game->m_players[i]);
    }
    _game->m_magic = 0xDEADBEEF; 
    free(_game);
}

/*****************************************/
static void PrintSumGame(Game *_game)
{   
    int i;
    printf("\n%sSum Game: ", MAGENTA);
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        printf("%s: %d points. ", PlayerName(_game->m_players[i]), PlayerTotalPoints(_game->m_players[i]));
    }
    printf("%s\n\n", NORMAL);
}

/*****************************************/

static int IsGameOver(Game *_game)
{
    int i;
    int gamePoints = 0;
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        gamePoints = PlayerTotalPoints(_game->m_players[i]);
        if(gamePoints >= MAX_GAME_POINTS)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************/

ADTErr RunGame(Game *_game) 
{ 
    Switch _switch;
    Round *_round;
    if(!IS_VALID(_game))
    {
        return ERR_NOT_INITIALIZED;
    }
	printf("%s\nLETS PLAY %s HAERTS %s %s\n", RED, HEART_SYMBOL, HEART_SYMBOL, NORMAL);
    while(!IsGameOver(_game))
    {
        _switch = (_game->m_numOfRounds + SWITCH_LEFT) % NUM_OF_SWITCH;        
        _round = RoundCreate(_game->m_players, _switch);
        RoundPlay(_round);
        _game->m_numOfRounds++;
        RoundDestroy(_round);
    }
    PrintSumGame(_game);
    return ERR_OK;
}
/*****************************************/
