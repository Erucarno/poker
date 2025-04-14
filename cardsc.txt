#include "cards.h"

#define CARDS_DECK_SIZE 52

char* reference_deck[] = {
        "\3   2   \3",
        "\4   2   \4",
        "\5   2   \5",
        "\6   2   \6",
        "\3   3   \3",
        "\4   3   \4",
        "\5   3   \5",
        "\6   3   \6",
        "\3   4   \3",
        "\4   4   \4",
        "\5   4   \5",
        "\6   4   \6",
        "\3   5   \3",
        "\4   5   \4",
        "\5   5   \5",
        "\6   5   \6",
        "\3   6   \3",
        "\4   6   \4",
        "\5   6   \5",
        "\6   6   \6",
        "\3   7   \3",
        "\4   7   \4",
        "\5   7   \5",
        "\6   7   \6",
        "\3   8   \3",
        "\4   8   \4",
        "\5   8   \5",
        "\6   8   \6",
        "\3   9   \3",
        "\4   9   \4",
        "\5   9   \5",
        "\6   9   \6",
        "\3  10   \3",
        "\4  10   \4",
        "\5  10   \5",
        "\6  10   \6",
        "\3 Jack  \3",
        "\4 Jack  \4",
        "\5 Jack  \5",
        "\6 Jack  \6",
        "\3 Queen \3",
        "\4 Queen \4",
        "\5 Queen \5",
        "\6 Queen \6",
        "\3 King  \3",
        "\4 King  \4",
        "\5 King  \5",
        "\6 King  \6",
        "\3  Ace  \3",
        "\4  Ace  \4",
        "\5  Ace  \5",
        "\6  Ace  \6"
};

char* shuffled_deck[] = {
        "\3   2   \3",
        "\4   2   \4",
        "\5   2   \5",
        "\6   2   \6",
        "\3   3   \3",
        "\4   3   \4",
        "\5   3   \5",
        "\6   3   \6",
        "\3   4   \3",
        "\4   4   \4",
        "\5   4   \5",
        "\6   4   \6",
        "\3   5   \3",
        "\4   5   \4",
        "\5   5   \5",
        "\6   5   \6",
        "\3   6   \3",
        "\4   6   \4",
        "\5   6   \5",
        "\6   6   \6",
        "\3   7   \3",
        "\4   7   \4",
        "\5   7   \5",
        "\6   7   \6",
        "\3   8   \3",
        "\4   8   \4",
        "\5   8   \5",
        "\6   8   \6",
        "\3   9   \3",
        "\4   9   \4",
        "\5   9   \5",
        "\6   9   \6",
        "\3  10   \3",
        "\4  10   \4",
        "\5  10   \5",
        "\6  10   \6",
        "\3 Jack  \3",
        "\4 Jack  \4",
        "\5 Jack  \5",
        "\6 Jack  \6",
        "\3 Queen \3",
        "\4 Queen \4",
        "\5 Queen \5",
        "\6 Queen \6",
        "\3 King  \3",
        "\4 King  \4",
        "\5 King  \5",
        "\6 King  \6",
        "\3  Ace  \3",
        "\4  Ace  \4",
        "\5  Ace  \5",
        "\6  Ace  \6"
};


void PrintReferenceDeck(){
    for(int i = 0; i < CARDS_DECK_SIZE; ++i) {
        printf("%s", *(reference_deck + i));
        printf("\n");
    }
}


void PrintShuffledDeck(){
    for(int i = 0; i < CARDS_DECK_SIZE; ++i) {
        printf("%s", *(shuffled_deck + i));
        printf("\n");
    }
}


void ShuffleTheDeck(){
    int j;
    char* temp[10];

    srand(time(NULL));

    for(int i = 0; i < CARDS_DECK_SIZE; i++){
        j = (rand() % 52);

        *temp = *(shuffled_deck + i);
        *(shuffled_deck + i) = *(shuffled_deck + j);
        *(shuffled_deck + j) = *temp;
    }
}


void DealTheCards(char** two_cards, int* two_cards_number, int* tag){

    for(int i = 0; i < 2; ++i) {
        *(two_cards) = *(shuffled_deck + *(tag));
        *(two_cards + i) = *(shuffled_deck + ( *(tag) + i ));

        for (int j = 0; j < CARDS_DECK_SIZE; ++j) {
            if (strcmp(*(two_cards + i), *(reference_deck + j)) == 0) {
                *(two_cards_number + i) = j;
                break;
            }
        }
    }

    *(tag) += 2;
}


void GetTheCardValue(int* cards_number,int* cards_value, int number_of_cards) {
    for(int i = 0; i < number_of_cards; ++i) {
        if (*(cards_number + i) > 47)
            *(cards_value + i) = 100;
        else
            *(cards_value + i) = ( *(cards_number + i) / 4 ) + 2;
    }
}


void CardsOnTheTable(char** cards_on_the_table, int* cards_on_the_table_number, int number_of_cards, int* tag){
    for(int i = 0; i < number_of_cards; ++i) {
        *(cards_on_the_table + i) = *(shuffled_deck + ( *tag + i ));

        for(int j = 0; j < CARDS_DECK_SIZE; ++j){
            if(strcmp(*(cards_on_the_table + i), *(reference_deck + j) ) == 0){
                *(cards_on_the_table_number + i) = j;
            }
        }
    }

    *tag += number_of_cards;
}
