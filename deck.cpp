/**
 * Author(s):     Team Six of Hearts:
 *                Ryan Nguyen (Taizuki), John Nguyen (johnguyn),
 *           	  Christian Hartman (Is-This-One-Avaiable), Caleb Funk (Funkyc02),
 *                Landon West (landonwest815), and Pablo Arancibia-Bazan (Pablo23117)
 * Course:        CS 3505, University of Utah, School of Computing
 * Assignment:    A9 - Educational App
 * File Contents:
 * Utilizes the Card class, to simulate a deck of 52 cards.
 * There are four suits: club, spade, diamond, and heart.
 * For each suit, there are cards with values from 1-9, then
 * there are three 10 cards with jack, queen, and king faces.
 * There are methods for drawing & adding cards to the deck.
 */
#include "deck.h"
#include <algorithm>
#include <random>

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
        Card* queen = new Card(suits[i],"queen");
        Card* king = new Card(suits[i],"king");

        //Add face cards
        cards.push_back(*jack);
        cards.push_back(*queen);
        cards.push_back(*king);
    }
}

Card Deck::drawCard() {
    //Get random card out of the vector
    int cardIndex = rand() % (cards.size() - 1);
    Card drawnCard = cards[cardIndex];
    //Erase the card from the vector and return that card
    cards.erase(cards.begin() + cardIndex);
    return drawnCard;
}

Card Deck::drawCard(string suit, int value) {
    //Create a new card and set it to the parameters
    Card* drawnCard = new Card(suit,value);
    //Iterate through vector
    for (Card card : cards) {
        //If the suit and value of a card is the same, set the drawnCard to that card and remove
        //it from the vector
        if (card == *drawnCard) {
            cards.erase(std::find(cards.begin(), cards.end(), card));
        }
    }
    //Return the drawn card
    return *drawnCard;
}

void Deck::addCard(string suit, int value) {
    Card* addedCard = new Card(suit, value);
    cards.push_back(*addedCard);
}

void Deck::addCard(string suit, string face) {
    Card* addedCard = new Card(suit, face);
    cards.push_back(*addedCard);
}

int Deck::getSize() {
    return cards.size();
}
