#include <stdio.h>
#include "time.h"
#include <stdlib.h>
#include <string.h>


#ifndef POKER_MECHANISM_H
#define POKER_MECHANISM_H


int SortAllCards   (int const* two_cards_value, int const* cards_on_the_table_value, int* array);


int IsOnePair       (int const* two_cards_value, int const* cards_on_the_table_value);              // two cards of the same value.
int IsTwoPairs      (int const* two_cards_value, int const* cards_on_the_table_value);              // two times two cards of same value.
int IsThreeOfKind   (int const* two_cards_value, int const* cards_on_the_table_value);              // three cards of the same value.
int IsStraight      (int const* two_cards_value, int const* cards_on_the_table_value);              // five cards with values in sequence.
int IsFlush         (int const* two_cards_number, int const* cards_on_the_table_number);            // five cards of same color.
int IsFullHouse     (int const* two_cards_value, int const* cards_on_the_table_value);              // three of a kind and one pair.
int IsFourOfKind    (int const* two_cards_value, int const* cards_on_the_table_value);              // four cards of the same value.
int IsStraightFlush (int const* two_cards_number, int const* cards_on_the_table_number);            // straight in which the cards have the same color.


#endif //POKER_MECHANISM_H
