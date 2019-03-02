#include <stdio.h>
#include "ADTDefs.h"
#include "vector.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "table.h"
#include "shapesandcolors.h"

#define FALSE 0
#define TRUE 1
#define NOT_INIT -1

#define MAGIC 0xBEEFBEEF
#define IS_VALID(A)             ((A) && (MAGIC == (A)->m_magic)) 

#define SUIT_VEC(PLAYER,SUIT)    (PLAYER->m_hand[(SUIT)])

struct Player
{
	char* m_name;
    PlayerType m_type;
	Vector* m_hand[NUM_OF_SUITS]; /* 4 suits */
	int m_pointsCount;
	int m_totalPoints;
	int m_magic;
};

/***************************************************************/
Player* PlayerCreate(char *_name, PlayerType _type)
{
	int i;	
	Player *player = malloc(sizeof(Player));
	if (!player)
	{
		return NULL;
	}
	
	for(i = 0; i < NUM_OF_SUITS; i++)
	{
		player->m_hand[i] = VectorCreate(CARDS_IN_HAND,0);
	}
	player->m_name = _name; 
	player->m_pointsCount = 0;
	player->m_totalPoints = 0;
	player->m_type = _type;
    player->m_magic = MAGIC;
	return player;
}
/***************************************************************/
void PlayerDestroy(Player *_player)
{
	if(!IS_VALID(_player))
	{
		return;
	}
	_player->m_magic = 0xDEADBEEF;
	free(_player);
}
/***************************************************************/
ADTErr PlayerAddPoints(Player *_player, int _points)
{
	if(!IS_VALID(_player))
	{
		return ERR_NOT_INITIALIZED;
	}
	_player->m_pointsCount += _points;	
	_player->m_totalPoints += _points;
	return ERR_OK;
}

/***************************************************************/
static void VectorBubbleSort(Vector *_vector)
{
	int i,j;
	int a1,a2;
	int len = VectorGetSize(_vector);
	if(!_vector)
	{
		return;
	}
	for(i = 0; i < len-1; ++i)
	{
		for(j = 1; j < len-i; ++j)
		{		
		    VectorGet(_vector, j, &a1);
		    VectorGet(_vector, j+1, &a2);
		    if( a1 > a2)
		    {
			    VectorSet(_vector,j,a2);
			    VectorSet(_vector,j+1,a1);
		    }
        }
	}
}
/***************************************************************/
ADTErr PlayerAddCard(Player *_player, int _card)
{
	ADTErr res;
    Vector *suitVector;	
	if(!IS_VALID(_player))
	{
		return ERR_NOT_INITIALIZED;
	}
	suitVector = SUIT_VEC(_player, GET_SUIT(_card));	
	res = VectorAdd(suitVector, _card);
	VectorBubbleSort(suitVector);
	return res;
}
/***************************************************************/
ADTErr PlayerReset(Player *_player)
{
	int card, i;
	Vector *suitVector;	
	if(!IS_VALID(_player))
	{
		return ERR_NOT_INITIALIZED;
	}	 
	_player->m_pointsCount = 0;
	for(i = 0; i < NUM_OF_SUITS; i++)
	{
		suitVector = SUIT_VEC(_player, i);			
		while(!IsVectorEmpty(suitVector))
		{		
		    VectorDelete(SUIT_VEC(_player,i), &card);
		}
	}
	return ERR_OK;
}
/***************************************************************/
int PlayerDoYouHaveCard(Player *_player, int _card)
{
    int i, cardIndex, vectorSize;
    if(!IS_VALID(_player))
    {
        return NOT_INIT;
    }
    
    vectorSize = VectorGetSize(SUIT_VEC(_player,GET_SUIT(_card)));
    for( i = 1; i <= vectorSize; i++)
    {
        VectorGet(SUIT_VEC(_player,GET_SUIT(_card)), i, &cardIndex);
        if (cardIndex == _card)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/***************************************************************/
void PlayerPrintHand(Player *_player)
{
    int i, j, card;
    int cardIndex = 1;
    printf("%s's Hand: ", PlayerName(_player));
    for (i = 0; i < NUM_OF_SUITS; i++)
    {
        for (j = 1; j <= VectorGetSize(_player->m_hand[i]); j++)
        {
            VectorGet(_player->m_hand[i], j, &card);
            printf("|%s%d.%s %s %s| ", GRAY, cardIndex, NORMAL, GetRankName(card), GetSuitName(card));
            cardIndex++;
        }
    }
    printf("\n");
 }
/***************************************************************/
static void PrintTable(Table *_table)
{
	int i, card;
	printf("\nTable: ");
	if(!VectorGetSize(_table->m_vec))
    {
		printf("|   |");
	}
 	for (i = 1; i <= VectorGetSize(_table->m_vec); i++)
    {
        VectorGet(_table->m_vec, i, &card);
        printf("|%s %s|  ", GetRankName(card), GetSuitName(card));
    }
    printf("\n");
}

/*************************HUMAN**********************************/	
/***************************************************************/
static void LeftShift(Vector *_vec, int _index)
{
	int i, deleted, leftItem;
	for (i = _index; i < VectorGetSize(_vec); i++)
	{	
        VectorGet(_vec, i+1, &leftItem);
		VectorSet(_vec, i, leftItem);
	}
	VectorDelete(_vec, &deleted);
}
/*****************************************************************/
static int FindCardIndex(Player *_player, int _card)
{
	int i, cardIndex, vectorSize;
	vectorSize = VectorGetSize(SUIT_VEC(_player,GET_SUIT(_card)));
    for( i = 1; i <= vectorSize; i++)
    {
        VectorGet(SUIT_VEC(_player,GET_SUIT(_card)), i, &cardIndex);
        if (cardIndex == _card)
        {
            return i;
        }
    }
	return NOT_INIT;
}
/****************************************************************/
static void RemoveCardFromHand(Player *_player, int _card)
{
	int index;
	Vector *vec;
	index = FindCardIndex(_player, _card);
	if (index >= 0)
	{
		vec = SUIT_VEC(_player,GET_SUIT(_card));
		LeftShift(vec, index);
	}
	else
	{
		printf("\n no index %d\n", _card);
	}
}

/***************************************************************/
static int ValidChoice(Player *_player, Table *_table, int _card)
{
	int leadCard;	
	Suit leadSuit;
	/* have card on hand?*/
	if (!PlayerDoYouHaveCard(_player, _card))
	{
		return 0;
	}
	/* is starter?*/
	if(!VectorGetSize(_table->m_vec))
    {
		return 1;
	}
	VectorGet(_table->m_vec, 1, &leadCard);
    leadSuit = GET_SUIT(leadCard); 
	/* missing lead suit?*/
	if (IsVectorEmpty(SUIT_VEC(_player,leadSuit)))
    {
		return 1;
	}
	/* got the rigth suit?*/
	return (leadSuit == GET_SUIT(_card));
}

/***************************************************************/
static int PlayerHandIndexToCard(Player *_player, int _handIndex)
{
    int i, suitCount, card;
    for (i = 0; i < NUM_OF_SUITS; i++)
    {
        suitCount = VectorGetSize(SUIT_VEC(_player, i));
        if (_handIndex <= suitCount) 
        {
            VectorGet(SUIT_VEC(_player, i), _handIndex, &card);
            return card;
        }
        _handIndex -= suitCount;
    }
    return NOT_INIT;
}

/***************************************************************/
static int HumanPlayCard(Player *_player, Table *_table)
{	
	int card, handIndex;
	PrintTable(_table);
	PlayerPrintHand(_player);
	printf("Play a card\n");
	scanf("%d", &handIndex);
    card = PlayerHandIndexToCard(_player, handIndex);
	while(!ValidChoice(_player, _table, card))
	{
		printf("Please try again\n");
		scanf("%d", &handIndex);
        card = PlayerHandIndexToCard(_player, handIndex);
	}
	RemoveCardFromHand(_player, card);
	return card;
}

/*************************PALY**********************************/
/***************************************************************/
static int PlayRandomCard(Player *_player)
{
    int i, card;
    for(i = NUM_OF_SUITS-1; i>=0; i--)
    {
        if (!IsVectorEmpty(SUIT_VEC(_player,i)))
        {
            VectorDelete(SUIT_VEC(_player,i), &card);
            return card;
        }
    }
    return  NO_MORE_CARDS;
}

/*************************************************************/
int PlayerPlayCard(Player *_player, Table *_table)
{
    int card, leadCard;
	Suit leadSuit;
    if(!IS_VALID(_player) || !_table)
    {
        return NOT_INIT;
    }
	/* (1#)If human */
	if (_player->m_type == HUMAN)
	{	
		card = HumanPlayCard(_player, _table);
        return card;
	}
    /* (2#)check if starter */
    if(!VectorGetSize(_table->m_vec))
    {
        card = PlayRandomCard(_player);
        return card;
    } 
    /* (3#)not starter */
    VectorGet(_table->m_vec, 1, &leadCard);
    leadSuit = GET_SUIT(leadCard); 

    if (!IsVectorEmpty(SUIT_VEC(_player,leadSuit)))
    {
        VectorDelete(SUIT_VEC(_player,leadSuit), &card);
        return card;
    }

    card = PlayRandomCard(_player);        
    return card;
}

/*************************SWITCH*********************************/
/***************************************************************/
static void HumanGetCardsForSwitch(Player *_player, SwitchCards *_switchCards)
{   
    int i, card, handIndex;
    printf("Choose %d cards you want to switch (Enter selected card number)\n", CARDS_IN_SWITCH);  
    for (i= 0; i < CARDS_IN_SWITCH; i++)
    {
        PlayerPrintHand(_player);
        scanf("%d", &handIndex);
        card = PlayerHandIndexToCard(_player, handIndex);
        _switchCards->m_cards[i] = card;
		RemoveCardFromHand(_player, card);
    }
}

/************************************************************************/
void PlayerGetCardsForSwitch(Player *_player, SwitchCards *_switchCards)
{
    int i;
	if (_player->m_type == HUMAN)
	{		
		HumanGetCardsForSwitch( _player, _switchCards);
	}
	else
	{
    	for (i= 0; i < CARDS_IN_SWITCH; i++)
    	{     
			_switchCards->m_cards[i] = PlayRandomCard(_player);
	    }
	}
}

/***************************************************************************/
void PlayerAddCardFormSwitch(Player *_player, SwitchCards *_switchCards)
{
    int i;
    for (i= 0; i < CARDS_IN_SWITCH; i++)
    {
        PlayerAddCard(_player, _switchCards->m_cards[i]);
    }
	if (_player->m_type == HUMAN)
	{	
		printf("\nCheck out your new hand:\n");
		PlayerPrintHand(_player);
	}
}
/**************************GET PLAYER INFO********************************/
char *PlayerName(Player *_player)
{
    return _player->m_name;
}

int PlayerPointsCount(Player *_player)
{
    return _player->m_pointsCount;
}

int PlayerTotalPoints(Player *_player)
{
    return _player->m_totalPoints;
}
