/***
 * Authors:     Team Six of Hearts
 * Members:     Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *              Landon West, and Pablo Arancibia-Bazan
 * Course:      CS 3505, University of Utah, School of Computing
 * Assignment:  A9 - Educational App
 *
 * Description: Utilizes the Card class to simulate a deck of 52 cards, including
 *              methods for drawing and adding cards to the deck.
 */

#include "deck.h"
#include <random>
#include <algorithm>

// Random numbers for drawing cards
std::random_device Deck::rd;
std::mt19937 Deck::gen(rd());

Deck::Deck()
{
    // Store the card suits and faces
    const QVector<QString> suits = {"C", "S", "D", "H"};
    const QVector<QString> faces = {"J", "Q", "K"};

    // Create cards for each suit (4 suits)
    for (int i = 0; i < 4; i++) {

        // Add an ace
        cards.push_back(Card(suits[i], 11));

        // Add all 9 numeric cards (2-10)
        for (int j = 2; j < 11; j++) {
            cards.push_back(Card(suits[i],j));
        }

        //Create face cards
        for (auto& face : faces) {
            cards.push_back(Card(suits[i], face));
        }
    }
}

Card Deck::drawCard() {
    // Retrieve a random index
    std::uniform_int_distribution<> dis(0, cards.size() - 1);
    int cardIndex = dis(gen);

    // Draw that card
    Card drawnCard = cards[cardIndex];

    // Erase the card from the vector and return that card
    cards.erase(cards.begin() + cardIndex);
    return drawnCard;
}

Card Deck::drawCard(QString suit, int value) {
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

void Deck::addCard(QString suit, int value) {
    cards.push_back(Card(suit, value));
}

void Deck::addCard(QString suit, QString face) {
    cards.push_back(Card(suit, face));
}

int Deck::getSize() {
    return cards.size();
}
