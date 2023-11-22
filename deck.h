#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

using std::vector;

class Deck
{
private:
    int size;
    vector<string> suits;
    vector<Card> cards;

public:
    ///@brief Create a deck of 52 cards
    Deck();

    ///@brief Draw a random card from the deck
    ///@return A random card
    Card drawCard();

    ///@brief Draw a card of the specified suit and value
    ///@param suit - The suit to be drawn
    ///@param value - the value to be drawn
    ///@return A card of the specified suit and value
    Card drawCard(string suit, int value);

    ///@brief Add a card of the specified suit and value
    ///@param suit - The suit to be added
    ///@param value - the value to be added
    void addCard(string suit, int value);

    ///@brief Reset the deck to it's original state
    void resetDeck();
};

#endif // DECK_H
