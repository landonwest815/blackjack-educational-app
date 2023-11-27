#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

using std::vector;

class Deck
{
private:
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

    ///@brief Add a card of the specified suit and face
    ///@param suit - The suit to be added
    ///@param face - the face to be added
    void addCard(string suit, string face);

    ///@brief Getter for deck size
    ///@return The size of the deck
    int getSize();

    ///@brief shuffles deck
    ///@return new reshuffle deck
    void reshuffle();
};

#endif // DECK_H
