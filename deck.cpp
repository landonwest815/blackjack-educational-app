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
#include <random>
#include <chrono>

// random numbers
std::random_device Deck::rd;
std::mt19937 Deck::gen(rd());

Deck::Deck()
{
    suits.push_back("C");
    suits.push_back("S");
    suits.push_back("D");
    suits.push_back("H");

    //Iterate through suits
    for (int i = 0; i < 4; i++) {

        Card* ace = new Card(suits[i], "A");
        cards.push_back(*ace);

        //Iterate through values
        for (int j = 2; j < 11; j++) {
            Card* addCard = new Card(suits[i],j);
            cards.push_back(*addCard);
        }

        //Create face cards
        Card* jack = new Card(suits[i],"J");
        Card* queen = new Card(suits[i],"Q");
        Card* king = new Card(suits[i],"K");

        //Add face cards
        cards.push_back(*jack);
        cards.push_back(*queen);
        cards.push_back(*king);
    }
}

Card Deck::drawCard() {
    //Get random card out of the vector
    std::uniform_int_distribution<> dis(0, cards.size() - 1);

    // Generate a random index
    int cardIndex = dis(gen);
    Card drawnCard = cards[cardIndex];

    // Erase the card from the vector and return that card
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
