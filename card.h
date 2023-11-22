#ifndef CARD_H
#define CARD_H

#include <string>

using std::string;

class Card
{
private:
    string suit;
    int value;
public:
    ///@brief Constructor for the Card class, initializes a new Card with a suit and value
    ///@param suit - The suit of the card
    ///@param value - The card's value, can be between 1 and 11
    Card(string suit, int value);

    ///@brief Getter for the suit
    ///@return the card's suit
    string getSuit();

    ///@brief Getter for the value
    ///@return the card's value
    int getValue();

    ///@brief Setter for the value
    ///@param newValue - The card's new value
    void setValue(int newValue);
};

#endif // CARD_H
