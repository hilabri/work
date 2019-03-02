#include "ADTDefs.h"
#include "deck.h"
#include "vector.h"
#include "shapesandcolors.h"
#define MAGIC 0xBEEFBEEF
#define IS_VALID(A)            ((A) && (MAGIC == (A)->m_magic)) 
#define FALSE 0
#define TRUE 1

const Card cards[] = 
                         { 
                         {TWO,HEART}, {TWO,DIAMOND},{TWO,CLUBS},{TWO,SPADE}, 
                         {THREE,HEART}, {THREE,DIAMOND},{THREE,CLUBS},{THREE,SPADE}, 
                         {FOUR,HEART}, {FOUR,DIAMOND},{FOUR,CLUBS},{FOUR,SPADE}, 
                         {FIVE,HEART}, {FIVE,DIAMOND},{FIVE,CLUBS},{FIVE,SPADE}, 
                         {SIX,HEART}, {SIX,DIAMOND},{SIX,CLUBS},{SIX,SPADE}, 
                         {SEVEN,HEART}, {SEVEN,DIAMOND},{SEVEN,CLUBS},{SEVEN,SPADE},
                         {EIGHT,HEART}, {EIGHT,DIAMOND},{EIGHT,CLUBS},{EIGHT,SPADE}, 
                         {NINE,HEART}, {NINE,DIAMOND},{NINE,CLUBS},{NINE,SPADE}, 
                         {TEN,HEART}, {TEN,DIAMOND},{TEN,CLUBS},{TEN,SPADE}, 
                         {PRINCE,HEART}, {PRINCE,DIAMOND},{PRINCE,CLUBS},{PRINCE,SPADE}, 
                         {QUEEN,HEART}, {QUEEN,DIAMOND},{QUEEN,CLUBS},{QUEEN,SPADE}, 
                         {KING,HEART}, {KING,DIAMOND},{KING,CLUBS},{KING,SPADE}, 
                         {ACE,HEART}, {ACE,DIAMOND},{ACE,CLUBS},{ACE,SPADE}, 
                         }; 


struct Deck
{
    Vector* m_vec;
    int m_magic;
};
/***************************************************************/
Deck* DeckCreate()
{
    int i;
    Deck* deck = malloc(sizeof(Deck));
    if(!deck)
    {
        return NULL;
    }
    deck->m_magic = MAGIC;
    deck->m_vec = VectorCreate(NUMBER_OF_CARDS,0);
    
    for(i = 0; i < NUMBER_OF_CARDS; i++)
    {
        VectorAdd(deck->m_vec, i);
    }
    return deck;
}
/***************************************************************/
ADTErr DeckShuffle(Deck *_deck)
{
    int i, j, veci, vecj;
    if(!IS_VALID(_deck))
    {
        return ERR_NOT_INITIALIZED;
    }
    if(IsVectorEmpty(_deck->m_vec))
    {
        return ERR_UNDERFLOW;
    }

    for(i = NUMBER_OF_CARDS; i > 1; i--)
    {
        j = (rand() % i) + 1; /* random index 1..i */
        VectorGet(_deck->m_vec, i, &veci);
        VectorGet(_deck->m_vec, j, &vecj);
        VectorSet(_deck->m_vec, i, vecj);
        VectorSet(_deck->m_vec, j, veci);
    }
    return ERR_OK;
}
/***************************************************************/
ADTErr DeckGetCard(Deck* _deck, int *_card)
{
    int cardIdex;
    ADTErr res;
    if(!(IS_VALID(_deck)) || (!_card))
    {
        return ERR_NOT_INITIALIZED;
    }
    
    if(IsVectorEmpty(_deck->m_vec))
    {
        return ERR_UNDERFLOW;
    }
    res = VectorDelete(_deck->m_vec,&cardIdex);
    *_card = cardIdex;

    return res;
}
/***************************************************************/
 void DeckDestroy(Deck *_deck)
{
    if (!_deck)
    {
        return;
    }
    VectorDestroy(_deck->m_vec);
    _deck->m_magic = 0xDEADBEEF;
    free(_deck);
}
/***************************************************************/
int IsCardValid(int _card)
{
    if(_card >= 0 && _card < NUMBER_OF_CARDS)
    {
        return TRUE;
    }
    return FALSE;
}
/***************************************************************/
char *GetSuitName(int _card)
{
    Suit suit = GET_SUIT(_card);
    switch (suit)
    {
    case HEART: return RED HEART_SYMBOL NORMAL;
    case SPADE: return GREEN SPADE_SYMBOL NORMAL;
    case DIAMOND: return RED DIAMOND_SYMBOL NORMAL;
    case CLUBS: return GREEN CLUB_SYMBOL NORMAL;
    default: return "error";
    } 
}
/***************************************************************/
char *GetRankName(int _card)
{   
    Rank rank = GET_RANK(_card);
    switch (rank)
    {
    case TWO: return "2";
    case THREE: return "3";
    case FOUR: return "4";
    case FIVE: return "5";
    case SIX: return "6";
    case SEVEN: return "7";
    case EIGHT: return "8";
    case NINE: return "9";
    case TEN: return "10";
    case PRINCE: return "P";
    case QUEEN: return "Q";
    case KING: return "K";
    case ACE: return "A";
    default: return "error";
    } 
}
/***************************************************************/
