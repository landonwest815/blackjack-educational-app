/***
 * Authors:     Team Six of Hearts
 * Members:     Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *              Landon West, and Pablo Arancibia-Bazan
 * Course:      CS 3505, University of Utah, School of Computing
 * Assignment:  A9 - Educational App
 *
 * Description: Header for a class representing a traditional playing card in a game of Blackjack.
 *              Cards can have values from 1 to 11, and faces for Jack, Queen, King, or Ace.
 */

#ifndef CARD_H
#define CARD_H

#include <QString>

///@brief Represents a playing card with a suit, value, and optional
///       face attribute. The card can be face up or face down.
class Card
{
private:

    ///@brief The card's suit (e.g., clubs, diamonds, hearts, spades).
    QString suit;

    ///@brief The card's value (1 to 11). Use the overloaded constructor for face cards.
    int value;

    ///@brief The card's face attribute, applicable if the value is 10.
    QString face;

    ///@brief Indicates if the card is facedown.
    bool facedown;

public:

    ///@brief Constructs a Card with a suit and value.
    ///@param suit - The suit of the card
    ///@param value - The card's value
    Card(QString suit, int value);

    ///@brief Constructs a Card with a suit and face for face cards.
    ///@param suit - The suit of the card
    ///@param face - The card's face
    Card(QString suit, QString face);

    ///@brief Returns the card's suit.
    QString getSuit();

    ///@brief Returns the card's face attribute.
    QString getFace();
    ///@brief Sets the card's face attribute.
    void setFace(QString newFace);

    ///@brief Returns the card's value.
    int getValue();
    ///@brief Sets the card's value.
    void setValue(int newValue);

    ///@brief Returns true if the card is facedown.
    bool getFaceDown();
    ///@brief Sets the card's facedown status.
    void setFaceDown(bool facedown);

    ///@brief Checks if two cards are equal based on suit, value, and face.
    bool operator== (const Card &c);
};

#endif // CARD_H
