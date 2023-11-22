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
