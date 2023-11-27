#include "model.h"

Model::Model() :
    bankTotal(1500),
    dealerTotal(0),
    userTotal(0) { }

int Model::hit(int userTotal) {
    Deck deck;
    Card nextCard = deck.drawCard();
    if(nextCard.getFace() == "A") {
        if(userTotal + nextCard.getValue() > 21) {
            userTotal = userTotal + 1;
        } else {
            userTotal = userTotal + 11;
        }
    } else {
        userTotal = userTotal + nextCard.getValue();
    }

    return userTotal;
}

int Model::stand(Card first, Card second) {
    return Model::getUserTotal(first, second);
}

int Model::doubleDown(int userTotal) {
    return Model::hit(userTotal);
}

bool Model::insurance(Card faceDown) {
    if(faceDown.getValue() == 10)
    {
        return true;
    } else {
        return false;
    }
}

int Model::getUserTotal(Card first, Card second) {
    return first.getValue() + second.getValue();
}

int Model::getDealerTotal(Card faceDown, Card faceUp) {
    return faceDown.getValue() + faceUp.getValue();
}
