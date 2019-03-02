#ifndef DECK_H_
#define DECK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "ADTDefs.h"
#include "card.h"

#define NUMBER_OF_CARDS 52

#define GET_SUIT(cardIndex) (cards[cardIndex].m_suit)
#define GET_RANK(cardIndex) (cards[cardIndex].m_rank)

typedef struct Deck Deck;

extern const Card cards[NUMBER_OF_CARDS];

Deck* DeckCreate(void); 
ADTErr DeckShuffle(Deck *_deck);
ADTErr DeckGetCard(Deck* _deck, int *_card);
void DeckDestroy(Deck *_deck);

int IsCardValid(int _card);
char *GetSuitName(int _card);
char *GetRankName(int _card);

#endif /* DECK_H_ */


