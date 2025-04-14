#include <stdio.h>
#include "time.h"
#include <stdlib.h>
#include <string.h>


#ifndef POKER_CARDS_H
#define POKER_CARDS_H


#define CARDS_DECK_SIZE 52


void PrintReferenceDeck     ();
void ShuffleTheDeck         ();
void PrintShuffledDeck      ();
void DealTheCards           (char** two_cards, int* two_cards_number, int* tag);
void GetTheCardValue        (int* cards_number, int* cards_value, int number_of_cards);
void CardsOnTheTable        (char** cards_on_the_table, int* cards_on_the_table_number, int number_of_cards, int* tag);


#endif //POKER_CARDS_H
