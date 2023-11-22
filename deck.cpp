#include "deck.h"

Deck::Deck()
{
    suits.push_back("club");
    suits.push_back("spade");
    suits.push_back("diamond");
    suits.push_back("heart");

    //Iterate through suits
    for (int i = 0; i < 4; i++) {

        //Iterate through values (where 1 = Ace)
        for (int j = 0; j < 9; j++) {
            Card* addCard = new Card(suits[i],j+1);
            cards.push_back(*addCard);
        }

        //Create face cards
        Card* jack = new Card(suits[i],"jack");
        Card* queen = new Card(suits[i],"jack");
        Card* king = new Card(suits[i],"jack");

        //Add face cards
        cards.push_back(*jack);
        cards.push_back(*queen);
        cards.push_back(*king);
    }
}
