#ifndef CARD_H
#define CARD_H

#include <string>

using std::string;

class Card
{
private:
    ///@brief The card's suit, for example a traditional card would have the suit of clubs, diamonds, hearts, or spades
    string suit;
    ///@brief A value between 1 and 11
    int value;
    ///@brief If the value is 10, the card can have a face attribute
    string face;
public:
    ///@brief Constructor for the Card class, initializes a new Card with a suit and value
    ///@param suit - The suit of the card
    ///@param value - The card's value, can be between 1 and 11 (cannot be 10, otherwise use the overloaded constructor)
    Card(string suit, int value);

    ///@brief Constructor for the Card class, initializes a new Card with a suit and value
    ///@param suit - The suit of the card
    ///@param face - The card's face if the value is 10
    Card(string suit, string face);

    ///@brief Getter for the suit
    ///@return the card's suit
    string getSuit();

    ///@brief Getter for the face
    ///@return the card's face
    string getFace();

    void setFace(string newFace);

    ///@brief Getter for the value
    ///@return the card's value
    int getValue();

    ///@brief Setter for the value
    ///@param newValue - The card's new value
    void setValue(int newValue);

    ///@brief Overloaded equals operator. Checks if suit, value, and face are the same
    bool operator== (const Card &c);
};

#endif // CARD_H
