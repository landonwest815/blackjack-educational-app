/***
 * Authors:     Team Six of Hearts
 * Members:     Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *              Landon West, and Pablo Arancibia-Bazan
 * Course:      CS 3505, University of Utah, School of Computing
 * Assignment:  A9 - Educational App
 *
 * Description: Class representing a traditional playing card in a game of Blackjack.
 *              Cards can have values from 1 to 11, and faces for Jack, Queen, King, or Ace.
 */
#include "card.h"

Card::Card(QString suit, int value)
    : suit(suit), value(value), facedown(false) {

    // Is an Ace
    if (value == 11) {
        this->face = "A";

    // Is Invalid - default to value of 2
    } else if (value < 2 || value > 10) {
        this->value = 2;
        this->face = "";

    // Is any common number
    } else {
        this->face = "";
    }
}

Card::Card(QString suit, QString face)
    : suit(suit), value(10), face(face), facedown(false) {}

QString Card::getSuit() {
    return suit;
}

QString Card::getFace() {
    return face;
}

void Card::setFace(QString newFace) {
    face = newFace;
}

int Card::getValue() {
    return value;
}

bool Card::getFaceDown() {
    return facedown;
}

void Card::setFaceDown(bool facedown) {
    this->facedown = facedown;
}

void Card::setValue(int newValue) {

    // Valid newValue
    if (newValue >= 1 && newValue <= 11) {
        value = newValue;
        if (newValue == 1) face = "A";

    // Invalid newValue - default to a value of 2
    } else {
        value = 2;
    }
}

bool Card::operator==(const Card &otherCard) {
    return suit == otherCard.suit && value == otherCard.value && face == otherCard.face;
}
