#include "card.h"

Card::Card(string suit, int value)
{
    this->suit = suit;

    //The value must be between 1 and 11. If it is not, the value will automatically be set to two.
    if (value >= 1 && value <= 11) {
        this->value = value;
    } else {
        this->value = 2;
    }
}

string Card::getSuit() {
    return suit;
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
