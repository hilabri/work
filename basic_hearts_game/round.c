#include <stdio.h>
#include "ADTDefs.h"
#include "player.h"
#include "round.h"
#include "trick.h"
#include "deck.h"
#include "card.h"
#include "shapesandcolors.h"

#define MAGIC 0xBEEFBEEF
#define IS_VALID(A)             ((A) && (MAGIC == (A)->m_magic)) 

#define FALSE 0
#define TRUE 1

#define STARTER_CARD 2

struct Round
{
	Player **m_players;
	Switch m_switch;
    int m_isHeartsBroken;
    int m_magic;
};

/***************************************************************/
Round *RoundCreate(Player **_players, Switch _switch)
{
    Round *newRound;
    newRound = malloc(sizeof(Round));
    if(!newRound)
    {
        return NULL;
    }
    newRound->m_players = _players;
    newRound->m_switch = _switch;
    newRound->m_isHeartsBroken = FALSE;
    newRound->m_magic = MAGIC;
    return newRound;
}
/***************************************************************/
void RoundDestroy(Round *_round)
{
    if(!IS_VALID(_round))
    {
        return;
    }
    _round->m_magic = 0xDEADBEEF;
    free(_round);
}
/***************************************************************/
static Deck *DealDeck(Round *_round)
{
    int i, j, card;
    Deck *deck = DeckCreate();
    DeckShuffle(deck);
    for(j = 0; j < NUM_OF_PLAYERS; j++)
    {
        for( i = 0; i < CARDS_IN_HAND; i++)
        {
            DeckGetCard(deck, &card);
            PlayerAddCard(_round->m_players[j], card);
        }
    }
    return deck;
}
/***************************************************************/
static void PrintSumTrick(Round *_round, Trick *_trick, int _trickCount, int _takerIndex, int _trickPoints)
{
    printf("%sTrick #%d: \n %s", CYAN, _trickCount, NORMAL);
    TrickPrintTable(_trick);
    printf("%sTaker: %s +%d Points%s\n", CYAN, PlayerName(_round->m_players[_takerIndex]), _trickPoints, NORMAL);
}
/***************************************************************/
static void PrintSumRound(Round *_round)
{
    int i;
    printf("%sSum Round: ", YELLOW);
    for (i = 0; i < NUM_OF_PLAYERS; i++)
    {
        printf("%s: %d Points. ", PlayerName(_round->m_players[i]), PlayerPointsCount(_round->m_players[i])); 
    }
    printf("%s\n", NORMAL);
}
/***************************************************************/
static void RoundSwitchCards(Round *_round)
{
    int i;
    SwitchCards switchCards[NUM_OF_PLAYERS];
    for(i = 0; i < NUM_OF_PLAYERS; i++)
    {
		PlayerGetCardsForSwitch(_round->m_players[i], &switchCards[i]);
    }
	for(i = 0; i < NUM_OF_PLAYERS; i++)
    {
    	PlayerAddCardFormSwitch(_round->m_players[i], &switchCards[(i+_round->m_switch) % NUM_OF_PLAYERS]);
	}
} 
/***************************************************************/
ADTErr RoundPlay(Round *_round)
{
    int i, haveCard, starter, takerIndex, trickPoints;
    Trick *trick;
    Deck *deck;
    if(!IS_VALID(_round))
    {
        return ERR_NOT_INITIALIZED;
    }
    deck = DealDeck(_round);
	printf("%s\nSWITCH CARDS%s\n", YELLOW, NORMAL);
    RoundSwitchCards(_round);
    for(i = 0; i < NUM_OF_PLAYERS; i++)
    {
        haveCard = PlayerDoYouHaveCard(_round->m_players[i], STARTER_CARD);
        if (haveCard)
        {
            starter = i;
        } 
    }
	printf("%s\nSTART ROUND%s\n", YELLOW, NORMAL);
    for(i = 0; i < CARDS_IN_HAND; i++)
    {
        trick = TrickCreate(_round->m_players, starter);
        TrickPlayAllPlayers(trick);
        takerIndex = TrickTakerIndex(trick);
        trickPoints = TrickGetPoints(trick);
        PlayerAddPoints(_round->m_players[takerIndex], trickPoints);
        PrintSumTrick(_round, trick, i+1, takerIndex, trickPoints);
        starter =takerIndex;
        TrickDestroy(trick);
    }
    PrintSumRound(_round);
    for(i = 0; i < NUM_OF_PLAYERS; i++)
    {
        PlayerReset(_round->m_players[i]);
    }
    DeckDestroy(deck);
    return ERR_OK;
}
/***************************************************************/
