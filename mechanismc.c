#include "mechanism.h"


int SortAllCards(int const* two_cards_value, int const* cards_on_the_table_value, int* array){
    int temp;

    array[0] = *(two_cards_value);
    array[1] = *(two_cards_value + 1);
    array[2] = *(cards_on_the_table_value);
    array[3] = *(cards_on_the_table_value + 1);
    array[4] = *(cards_on_the_table_value + 2);
    array[5] = *(cards_on_the_table_value + 3);
    array[6] = *(cards_on_the_table_value + 4);

    for(int b = 0; b < 7; ++b){
        for(int c = b + 1; c < 7; ++c){
            if(array[b] > array[c]){
                temp = array[b];
                array[b] = array[c];
                array[c] = temp;
            }
        }
    }

    return array[6];
}


int IsOnePair(int const* two_cards_value, int const* cards_on_the_table_value){
    int pair_value = 0;
    int array[7];

    SortAllCards(two_cards_value, cards_on_the_table_value, array);

    for (int i = 0; i < 7; ++i) {
        for (int j = i+1; j < 7; ++j) {
            if(array[i] == array[j] && array[i] > pair_value && pair_value > 0){
                pair_value = array[i];
            }
            if(array[i] == array[j]){
                pair_value = array[i];
            }
        }
    }

    if(pair_value > 0) {
        printf("There is one pair:\n");
        printf("One pair value: %d\n", pair_value);
        return pair_value;
    }
    else
        return 0;
}


int IsTwoPairs(int const* two_cards_value, int const* cards_on_the_table_value){
    int array [7];
    int check_point = 0;
    int first_pair_value = 0;
    int second_pair_value = 0;

    SortAllCards(two_cards_value, cards_on_the_table_value, array);

    for (int i = 0; i < 7; ++i) {
        for (int j = i+1; j < 7; ++j) {
            if(array[i] == array[j] && array[i] > first_pair_value){
                first_pair_value = array[i];
                check_point ++;
            }
        }
        if(check_point == 1)
            second_pair_value = first_pair_value;
    }

    //for(int b = 0; b < 2; ++b){
    //    for(int c = 0; c < size; ++c){
    //        if( *(two_cards_value + b) == *(cards_on_the_table_value + c) && *(two_cards_value + b) > first_pair_value ){
    //            first_pair_value = *(two_cards_value + b);
    //            check_point++;
    //        }
    //    }
    //    if(check_point == 1)
    //        second_pair_value = first_pair_value;
    //}

    if(first_pair_value > 0 && second_pair_value > 0 && first_pair_value != second_pair_value){
        printf("There are two pairs\n");
        printf("Fisrt pair value = %d\n", first_pair_value);
        printf("Second pair value = %d\n", second_pair_value);

        if(first_pair_value > second_pair_value)
            return first_pair_value;
        else
            return second_pair_value;
    }
    else
        return 0;
}


int IsThreeOfKind(int const* two_cards_value, int const* cards_on_the_table_value){
    int array [7];
    int check_point = 0;
    int three_of_kind_value = 0;
    int final_value = 0;

    SortAllCards(two_cards_value, cards_on_the_table_value, array);

    for (int i = 0; i < 7; ++i) {
        for (int j = i+1; j < 7; ++j) {
            if(array[i] == array[j] && array[i] > three_of_kind_value){
                three_of_kind_value = array[i];
                check_point = 1;
                continue;
            }
            if(array[i] == array[j] && array[i] == three_of_kind_value && check_point == 1){
                three_of_kind_value = array[i];
                check_point = 2;
                continue;
            }
            if(array[i] == array[j] && array[i] == three_of_kind_value && check_point == 2){
                three_of_kind_value = array[i];
                check_point = 3;
                break;
            }
        }
        if(check_point == 3)
            final_value = three_of_kind_value;
    }

    //for(int b = 0; b < 2; ++b){
    //    for(int c = 0; c < size; ++c){
    //        if( *(two_cards_value + b) == *(cards_on_the_table_value + c) && *(two_cards_value + b) > three_of_kind_value ){
    //            three_of_kind_value = *(two_cards_value + b);
    //            check_point = 1;
    //            continue;
    //        }
    //        if( *(two_cards_value + b) == *(cards_on_the_table_value + c) && *(two_cards_value + b) == three_of_kind_value ){
    //            three_of_kind_value = *(two_cards_value + b);
    //            check_point++;
    //        }
    //    }
    //    if(check_point == 2)
    //        final_value = three_of_kind_value;
    //}

    if(final_value > 0){
        printf("There are three of a kind\n");
        printf("Three of kind value: %d\n", final_value);
        return final_value;
    }
    else
        return 0;
}


int IsStraight(int const* two_cards_value, int const* cards_on_the_table_value){
    int array[7];
    int value_array[5];
    int check_point = 0;

    SortAllCards(two_cards_value, cards_on_the_table_value, array);

    //Sorting
    //if(*two_cards_value > *(two_cards_value + 1)) {
    //    temp = *two_cards_value;
    //    *two_cards_value = *(two_cards_value + 1);
    //    *(two_cards_value + 1) = temp;
    //}

    printf("Cards value: ");
    for (int i = 0; i < 7; ++i) {
        printf("%d  ", array[i]);
    }

    printf("\n");

    for(int d = 0; d < 2; ++d){
        if(array[d] + 1 == array[d+1] && array[d+1] + 1 == array[d+2] && array[d+2] + 1 == array[d+3] &&
                array[d+3] + 1 == array[d+4] && array[d+4] + 1 == array[d+5]){

            value_array[0] = array[d+1];
            value_array[1] = array[d+2];
            value_array[2] = array[d+3];
            value_array[3] = array[d+4];
            value_array[4] = array[d+5];

            check_point = 1;
        }
    }

    if(check_point == 1){
        printf("There is a straight\n");
        printf("Straight values: %d  %d  %d  %d  %d\n\n", value_array[0], value_array[1], value_array[2],
               value_array[3], value_array[4]);

        return value_array[4];
    }
    else
        return 0;
}


int IsFlush(int const* two_cards_number, int const* cards_on_the_table_number){
    int array[7];
    int number_array[5];
    int check_point = 0;

    SortAllCards(two_cards_number, cards_on_the_table_number, array);

    for(int d = 0; d < 2; ++d){
        if( (array[d+1] - array[d]) % 4 == 0 && (array[d+2] - array[d+1]) % 4 == 0 && (array[d+3] - array[d+2]) % 4 == 0
            && (array[d+4] - array[d+3]) % 4 == 0  && (array[d+5] - array[d+4]) % 4 == 0){

            number_array[0] = array[d+1];
            number_array[1] = array[d+2];
            number_array[2] = array[d+3];
            number_array[3] = array[d+4];
            number_array[4] = array[d+5];

            check_point = 1;
        }
    }

    if(check_point == 1){
        printf("There is flush\n");
        printf("Flush card numbers: %d  %d  %d  %d  %d\n\n", number_array[0], number_array[1], number_array[2],
               number_array[3], number_array[4]);

        return 1;
    }
    else
        return 0;
}


int IsFullHouse(int const* two_cards_value, int const* cards_on_the_table_value){
    if(IsTwoPairs(two_cards_value, cards_on_the_table_value) && IsThreeOfKind(two_cards_value, cards_on_the_table_value)){
        printf("There is a full house\n");
        return 1;
    }
    else
        return 0;
}


int IsFourOfKind(int const* two_cards_value, int const* cards_on_the_table_value){
    int array [7];
    int check_point = 0;
    int four_of_kind_value = 0;
    int final_value = 0;

    SortAllCards(two_cards_value, cards_on_the_table_value, array);

    for (int i = 0; i < 7; ++i) {
        for (int j = i+1; j < 7; ++j) {
            if(array[i] == array[j] && array[i] > four_of_kind_value){
                four_of_kind_value = array[i];
                check_point = 1;
                continue;
            }
            if(array[i] == array[j] && array[i] == four_of_kind_value && check_point == 1){
                four_of_kind_value = array[i];
                check_point = 2;
                continue;
            }
            if(array[i] == array[j] && array[i] == four_of_kind_value && check_point == 2){
                four_of_kind_value = array[i];
                check_point = 3;
                continue;
            }
            if(array[i] == array[j] && array[i] == four_of_kind_value && check_point == 3){
                four_of_kind_value = array[i];
                check_point = 4;
                break;
            }
        }
        if(check_point == 4)
            final_value = four_of_kind_value;
    }

    //for(int b = 0; b < 2; ++b){
    //    for(int c = 0; c < size; ++c){
    //        if( *(two_cards_value + b) == *(cards_on_the_table_value + c) && *(two_cards_value + b) > three_of_kind_value ){
    //            three_of_kind_value = *(two_cards_value + b);
    //            check_point = 1;
    //            continue;
    //        }
    //        if( *(two_cards_value + b) == *(cards_on_the_table_value + c) && *(two_cards_value + b) == three_of_kind_value ){
    //            three_of_kind_value = *(two_cards_value + b);
    //            check_point++;
    //        }
    //    }
    //    if(check_point == 2)
    //        final_value = three_of_kind_value;
    //}

    if(final_value > 0){
        printf("There are four of a kind\n");
        printf("Four of kind value: %d\n", final_value);
        return final_value;
    }
    else
        return 0;
}


int IsStraightFlush(int const* two_cards_number, int const* cards_on_the_table_number){
    int array[7];
    int number_array[5];
    int check_point = 0;

    SortAllCards(two_cards_number, cards_on_the_table_number, array);

    for(int d = 0; d < 2; ++d){
        if( abs(array[d+1] - array[d]) == 4 && abs(array[d+2] - array[d+1]) == 4 && abs(array[d+3] - array[d+2]) == 4
            && abs(array[d+4] - array[d+3]) == 4  && abs(array[d+5] - array[d+4]) == 4){

            number_array[0] = array[d+1];
            number_array[1] = array[d+2];
            number_array[2] = array[d+3];
            number_array[3] = array[d+4];
            number_array[4] = array[d+5];

            check_point = 1;
        }
    }

    if(check_point == 1){
        printf("There is straight flush\n");
        printf("Straight flush card numbers: %d  %d  %d  %d  %d\n\n", number_array[0], number_array[1], number_array[2],
               number_array[3], number_array[4]);

        return 1;
    }
    else
        return 0;
}
