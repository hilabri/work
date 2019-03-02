#ifndef CARD_H_
#define CARD_H_


typedef enum
{
    TWO = 0,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    PRINCE,
    QUEEN,
    KING,
    ACE
}Rank;

typedef enum 
{
    HEART = 0,
    DIAMOND,
    CLUBS,
    SPADE,
	NUM_OF_SUITS
}Suit;

typedef struct Card Card;

struct Card
{
    Rank m_rank;
    Suit m_suit;
};

#endif /* CARD_H_ */
