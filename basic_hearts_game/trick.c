#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"
#include "vector.h"
#include "player.h"
#include "table.h"
#include "trick.h"
#include "deck.h"

#define MAGIC 0xBEEFBEEF
#define IS_VALID(A)             ((A) && (MAGIC == (A)->m_magic)) 
#define FALSE 0
#define TRUE 1
#define NOT_INIT -1

#define TABLE(T)  (T->m_table.m_vec)

struct Trick
{
	Table m_table;
    Player **m_players;
	int m_starter;
	int m_magic;
};
/***************************************************************/
Trick *TrickCreate(Player **_players, int _starter)
{
    Trick *trick = malloc(sizeof(Trick));
	if(!trick)
	{
		return NULL;
	}
	TABLE(trick) = VectorCreate(NUM_OF_PLAYERS,0);
    trick->m_starter = _starter;
    trick->m_players = _players;
    trick->m_magic = MAGIC;
	return trick;
}
/***************************************************************/
void TrickDestroy(Trick *_trick)
{
	if(!IS_VALID(_trick))
	{
		return;
	}
	VectorDestroy(TABLE(_trick));
	_trick->m_magic = 0xDEADBEEF;	
	free(_trick);
}
/***************************************************************/
void TrickPlayAllPlayers(Trick *_trick)
{
	int i;
    int card;
	if(!IS_VALID(_trick))
    { 
        return;
    }
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        card = PlayerPlayCard(_trick->m_players[(_trick->m_starter+i) % NUM_OF_PLAYERS], &_trick->m_table);
        if(IsCardValid(card))
        {
            VectorAdd(TABLE(_trick), card);
        }
    }
}
/***************************************************************/
int TrickTakerIndex(Trick *_trick)
{
	int i, card, rank;
    int maxIndex = -1;	
	int maxRank = -1;
	if(!IS_VALID(_trick))
    { 
        return NOT_INIT;
    }   
    for(i = 1; i <= NUM_OF_PLAYERS; i++)
    {
        VectorGet(TABLE(_trick), i, &card);
        rank = GET_RANK(card);
        if (rank > maxRank)
        {
            maxRank = rank;
            maxIndex = i-1;
        }
    }
    return (_trick->m_starter+maxIndex) % NUM_OF_PLAYERS;
}
/***************************************************************/
int TrickGetPoints(Trick *_trick)
{
	int i;
    int card;
    int points = 0;    
	if(!IS_VALID(_trick))
    {   
        return NOT_INIT;
    }
    for(i = 1; i <= NUM_OF_PLAYERS; i++)
    {
        VectorGet(TABLE(_trick), i, &card);
        if (GET_SUIT(card) == HEART)
        {
                points++;
        }
        if (GET_SUIT(card)== SPADE && GET_RANK(card) == QUEEN)
        {
            points+=13;
        }
    }
    return points;
}
/***************************************************************/
void TrickPrintTable(Trick *_trick)
{
    int i, card;
    if(!IS_VALID(_trick))
    {   
        return; 
    }    
    for (i = 1; i <= VectorGetSize(TABLE(_trick)); i++)
    {
        VectorGet(TABLE(_trick), i, &card);
        printf("|%s %s|  ", GetRankName(card), GetSuitName(card));
    }
    printf("\n");
}	
/***************************************************************/
