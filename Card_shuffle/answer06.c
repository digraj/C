#include "answer06.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
// You may add more functions in this file.

// do NOT modify any #ifdef or #endif
// they are used for giving partial credits

// The following line must be above divideDeck
#ifdef TEST_DIVIDE 
void divideDeck(CardDeck origdeck, 
		CardDeck * upperdeck, CardDeck * lowerdeck)
{
  // if the original deck has n cards, there are n - 1 pairs of
  // upper and low decks: 
  // 1.   upper deck has 1 card,      low deck has n - 1 cards
  // 2.   upper deck has 2 cards,     low deck has n - 2 cards
  // 3.   upper deck has 3 cards,     low deck has n - 3 cards
  // ...
  // n-1. upper deck has n - 1 cards, low deck has 1 cards
  
  // upperdeck[0] should store the first card in the original deck
  // upperdeck[1] should store the top two cards in the original deck
  // upperdeck[2] should store the top three cards in the original deck

  // be careful that in CardDeck, cards is an array (thus a pointer)

	int j = origdeck.size - 1 ;
	int i; 	
	int k;
	int l = 0; 
	for (i = 0; i <= j; i++) {		
		upperdeck[i].size = 0 ;
		lowerdeck[i].size = 0 ;		
		for (k = 0; k <= i; k++) {		
			upperdeck[i].cards[k] = origdeck.cards[k] ;
			upperdeck[i].size += 1 ;
		}
		for (k = (i + 1); k <= j; k++) {
			lowerdeck[i].cards[l++] = origdeck.cards[k] ;
			lowerdeck[i].size += 1 ;		
		}
		l = 0 ;
	}
}
#endif
// The previous line must be below divideDeck

#ifdef TEST_INTERLEAVE
// reuse what you wrote for PE05

void helperfcn(CardDeck deck, CardDeck upperdeck, CardDeck lowerdeck, int upper_size, int lower_size, int i) 
{	
	int j = 0 ;
	if (upper_size == 0) {
		for (j = 0; j < lower_size; j++) {
			deck.cards[i + j] = lowerdeck.cards[lowerdeck.size - lower_size + j] ;
		}
		printDeck(deck) ;	
		printf("\n") ;
		return ;
	}
	else if (lower_size == 0) {
		for (j = 0; j < upper_size; j++) {
			deck.cards[i + j] = upperdeck.cards[upperdeck.size - upper_size + j] ;
		}
		printDeck(deck) ;
		printf("\n") ;
		return ;
	} 
	if (upper_size > 0) {	
		deck.cards[i] = upperdeck.cards[upperdeck.size - upper_size] ;
		helperfcn(deck, upperdeck, lowerdeck, upper_size - 1, lower_size, i + 1) ; 
	}
	if (lower_size > 0) {	
		deck.cards[i] = lowerdeck.cards[lowerdeck.size - lower_size] ;
		helperfcn(deck, upperdeck, lowerdeck, upper_size, lower_size - 1, i + 1) ;		
	}	
	return ;
}

void interleave(CardDeck upperdeck, 
		CardDeck lowerdeck)
{
  // create a new deck storing the result
  // this new deck's size should be the sum of the
  // sizes of upperdeck and lowerdeck
	CardDeck deck ;
	deck.size = upperdeck.size + lowerdeck.size ;
  // call the helper function with appropriate arguments
	int i = 0 ;	
	helperfcn(deck, upperdeck, lowerdeck, upperdeck.size, lowerdeck.size, i) ;
}
#endif

#ifdef TEST_SHUFFLE
void shuffle(CardDeck origdeck)
{
  // origdeck contains the number of cards
  // the number of upper-low deck pairs should be 
  //     the number of cards - 1
  // this function has the following steps:
  //    calculate the number of upper-low deck pairs
  //    allocate enough memory to accommodate the pairs
  //    call divideDeck to find these pairs
  //    for each pair of upper-lower deck, interleave the cards
  //    release the allocated memory
  // 
  // The amount of memory allocated in this function must be
  // determined based on the size of the original deck. The program
  // may fail if the allocated memory has a fixed size (some students
  // like to use 1000 but nobody could explain why 1000).  If you do
  // not know how much memory to allocate, please refer to PE05's
  // README.
  // 
  // You will lose 50% points if the amount of allocated memory is not
  // determined by the size of the original deck.  To be more precise,
  // you will use 50% point if you use 100, 1000, 10000 (or any other
  // fixed number) without clear explanation.
  //

  // the following code gives you some hints. You may make any changes
  // as appropriate;
  int numpairs = origdeck.size - 2;
  
  CardDeck * upperdeck = NULL;
  CardDeck * lowerdeck = NULL;

  // allocate memory

	upperdeck = (CardDeck *)malloc(sizeof(CardDeck) * (origdeck.size));
	lowerdeck = (CardDeck *)malloc(sizeof(CardDeck) * (origdeck.size));
  	if (upperdeck == NULL)	{
			return ;
   		}
 	if (lowerdeck == NULL)	{
		free(upperdeck) ;
		return ;
	} 

  // call divideDeck to fill upperdeck and lowerdeck 
	divideDeck(origdeck, upperdeck, lowerdeck) ;
  while (numpairs >= 0)
    {
      // for each pairs call interleave
    	interleave(upperdeck[numpairs], lowerdeck[numpairs]) ;
	numpairs-- ;
    }

	free(upperdeck) ;
	free(lowerdeck) ;
} 
#endif
