#include "model.h"

Model::Model() :
    bankTotal(1500),
    dealerTotal(0),
    userTotal(0),
    bet(100),
    win(true){ }

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
    bankTotal -= this->bet;
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

int Model::getbankTotal() {
    return bankTotal;
}

int Model::updateBankTotal(int bet) {
    if(win) {
        bankTotal += bet;
    } else {
        bankTotal -= bet;
    }
    return bankTotal;
}
void Model::userBlackJack(Card first, Card second) {
    if(first.getValue() + second.getValue() == 21)
        this->bankTotal += (this->bet * 1.5);
}

void Model::dealerBlackJack(Card faceDown, Card faceUp) {
    if(faceDown.getValue() + faceUp.getValue() == 21)
        this->bankTotal -= this->bet;
}
