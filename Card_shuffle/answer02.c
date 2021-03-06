#include "answer02.h"
#include <stdio.h>
#include <stdlib.h>
// You can add more functions to this file.
//
// If you allocate for space, you are responsible for freeing the space
// 
// You will lose 50% points if the amount of allocated memory is not
// determined by the size of the original deck.  To be more precise,
// you will lose 50% point if you use 100, 1000, 10000 (or any other
// fixed number) without clear explanation.
//

//CODE TRANSFERRED FROM PE05 AND PE06

void divideDeck(CardDeck origdeck, CardDeck * upperdeck, CardDeck * lowerdeck, int k)
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
	int m;
	int l = 0; 
	for (i = 0; i <= j; i++) {		
		upperdeck[i].size = 0 ;
		lowerdeck[i].size = 0 ;		
		for (m = 0; m <= i; m++) {		
			upperdeck[i].cards[m] = origdeck.cards[m] ;
			upperdeck[i].size += 1 ;
		}
		for (m = (i + 1); m <= j; m++) {
			lowerdeck[i].cards[l++] = origdeck.cards[m] ;
			lowerdeck[i].size += 1 ;		
		}
		l = 0 ;
	}
}


void helperfcn(CardDeck deck, CardDeck upperdeck, CardDeck lowerdeck, int upper_size, int lower_size, int i, int k) 
{	
	int j = 0 ;
	if (upper_size == 0) {
		for (j = 0; j < lower_size; j++) {
			deck.cards[i + j] = lowerdeck.cards[lowerdeck.size - lower_size + j] ;
		}
		if (k > 0) { 
			k-- ; 
			repeatShuffle(deck, k) ; 
		}
		//printDeck(deck) ;
		return ;
	}
	else if (lower_size == 0) {
		for (j = 0; j < upper_size; j++) {
			deck.cards[i + j] = upperdeck.cards[upperdeck.size - upper_size + j] ;
		}
		if (k > 0) { 
			k-- ; 
			repeatShuffle(deck, k) ; 
		} 
		//printDeck(deck) ;
		return ;
	} 
	if (upper_size > 0) {	
		deck.cards[i] = upperdeck.cards[upperdeck.size - upper_size] ;
		helperfcn(deck, upperdeck, lowerdeck, upper_size - 1, lower_size, i + 1, k) ; 
	}
	if (lower_size > 0) {	
		deck.cards[i] = lowerdeck.cards[lowerdeck.size - lower_size] ;
		helperfcn(deck, upperdeck, lowerdeck, upper_size, lower_size - 1, i + 1, k) ;		
	}	
	return ;
}

void interleave(CardDeck upperdeck, CardDeck lowerdeck, int k)
{
  // create a new deck storing the result
  // this new deck's size should be the sum of the
  // sizes of upperdeck and lowerdeck
	CardDeck deck ;
	deck.size = upperdeck.size + lowerdeck.size ;
  // call the helper function with appropriate arguments
	int i = 0 ;	
	helperfcn(deck, upperdeck, lowerdeck, upperdeck.size, lowerdeck.size, i, k) ;
}

/*
void shuffle(CardDeck origdeck)
{
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
*/
//END OF CODE TRANSFER

void repeatShuffle(CardDeck origdeck, int k)
{
  	// origdeck contains the number of cards
  	// the number of upper-low deck pairs should be 
  	//     the number of cards - 1
  	// this function has the following steps:
  	//    if (k <= 0), no shuffling, print the only possible outcome
  	//    otherwise, 
  	//       for each pair of upper and lower decks, interleave the cards,
  	//           when the interleaving is complete, have to perform another 
  	//           k-1 rounds of shuffling with the new deck
  	//
  	// Print only the results obtained after k rounds of shuffling
  	// see interleave in PE05 and shuffle in PE06 for more details
	if (k > 0) {
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
	
		divideDeck(origdeck, upperdeck, lowerdeck, k) ;

	 	while (numpairs >= 0)
	    	{
	      	// for each pairs call interleave
	    		interleave(upperdeck[numpairs], lowerdeck[numpairs], k) ;
			numpairs-- ;
	    	}

		free(upperdeck) ;
		free(lowerdeck) ;
	}
	else {
		printDeck(origdeck) ;
		return ;
	}

}

