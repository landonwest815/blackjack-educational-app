/**
 * Author(s):     Team Six of Hearts:
 *                Ryan Nguyen (Taizuki), John Nguyen (johnguyn),
 *           	  Christian Hartman (Is-This-One-Avaiable), Caleb Funk (Funkyc02),
 *                Landon West (landonwest815), and Pablo Arancibia-Bazan (Pablo23117)
 * Course:        CS 3505, University of Utah, School of Computing
 * Assignment:    A9 - Educational App
 * File Contents:
 * Class that has attributes of a traditional playing card in a game of Blackjack.
 * If the value is 10, then the card can have a face of Jack, Queen, or King.
 * If the value is not 10, then the card does not have a face.
 */
#include "card.h"

Card::Card(string suit, int value)
{
    this->suit = suit;
    this->face = "";

    //The value must be between 1 and 11. If it is not, the value will automatically be set to two.
    if (value >= 1 && value <= 11 && value != 10) {
        this->value = value;
    } else {
        this->value = 2;
    }
}

Card::Card(string suit, string face) {
    this->suit = suit;
    this->value = 10;
    this->face = face;
}

string Card::getSuit() {
    return suit;
}

string Card::getFace() {
    return face;
}

int Card::getValue() {
    return value;
}

void Card::setValue(int newValue) {
    //The value must be between 1 and 11. If it is not, the value will automatically be set to two.
    if (newValue >= 1 && newValue <= 11) {
        value = newValue;
    } else {
        value = 2;
    }
}

bool Card::operator==(const Card &c) {
    if (suit == c.suit && value == c.value && face == c.face) {
        return true;
    }
    return false;
}
