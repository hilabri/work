#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"
#include "card.h"
#include "table.h"

#define FALSE 0
#define TRUE 1
#define NOT_INIT -1
#define NO_MORE_CARDS -2

#define CARDS_IN_HAND  13
#define CARDS_IN_SWITCH 3

typedef struct Player Player;

typedef enum  
{
    HUMAN = 0,
    COMPUTER
} PlayerType;

typedef struct
{
    int m_cards[CARDS_IN_SWITCH];            
} SwitchCards;


Player* PlayerCreate(char *_name, PlayerType _type);
void PlayerDestroy(Player *_Player);

ADTErr PlayerAddPoints(Player *_player, int _points);
ADTErr PlayerAddCard(Player *_player, int _card);
int PlayerPlayCard(Player *_player, Table *_table);
ADTErr PlayerReset(Player *_player);
int PlayerDoYouHaveCard(Player *_player, int _card);

void PlayerGetCardsForSwitch(Player *_player, SwitchCards *_switchCards);
void PlayerAddCardFormSwitch(Player *_player, SwitchCards *_switchCards);


char *PlayerName(Player *_player);
void PlayerPrintHand(Player *_player);
int PlayerPointsCount(Player *_player);
int PlayerTotalPoints(Player *_player);

#endif /* PLAYER_H_ */
