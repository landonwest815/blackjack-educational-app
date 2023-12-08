/***
 * Authors:     Team Six of Hearts
 * Members:     Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *              Landon West, and Pablo Arancibia-Bazan
 * Course:      CS 3505, University of Utah, School of Computing
 * Assignment:  A9 - Educational App
 *
 * Description: Header for the Deck class that utilizes the Card class.
 *              It simulates a deck of 52 cards, including methods for
 *              drawing and adding cards to the deck.
 */

#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <random>
#include <QVector>
#include <QString>

///@brief Represents a deck of playing cards, allowing for drawing and adding of cards.
///       Supports drawing random cards or specific cards by suit and value.
class Deck
{
private:

    ///@brief Available suits in the deck.
    QVector<QString> suits;

    ///@brief Holds the cards in the deck.
    QVector<Card> cards;

    ///@brief Random device for card shuffling.
    static std::random_device rd;

    ///@brief Generator for randomization.
    static std::mt19937 gen;

public:

    ///@brief Constructs a deck with 52 cards.
    Deck();

    ///@brief Returns a random card from the deck.
    Card drawCard();

    ///@brief Returns a specific card by suit and value.
    ///@param suit - The suit to be drawn
    ///@param value - the value to be drawn
    Card drawCard(QString suit, int value);

    ///@brief Adds a card with specified suit and value to the deck.
    ///@param suit - The suit of the card to be added
    ///@param value - the value of the card to be added
    void addCard(QString suit, int value);

    ///@brief Adds a face card with specified suit to the deck.
    ///@param suit - The suit of the card to be added
    ///@param face - the face of the card to be added
    void addCard(QString suit, QString face);

    ///@brief Returns the current size of the deck.
    int getSize();
};

#endif // DECK_H
