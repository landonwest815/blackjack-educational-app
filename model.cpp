#include "model.h"
#include "deck.h"

Model::Model() :
    bankTotal(1500),
    dealerTotal(0),
    userTotal(0),
    bet(0),
    win(true){ }

Card Model::userHit() {
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

    return nextCard;
}

Card Model::dealerHit() {
    Card nextCard = deck.drawCard();

    if(nextCard.getFace() == "A") {
        if(dealerTotal + nextCard.getValue() > 21) {
            dealerTotal = dealerTotal + 1;
        } else {
            dealerTotal = dealerTotal + 11;
        }
    } else {
        dealerTotal = dealerTotal + nextCard.getValue();
    }

    return nextCard;
}
int Model::stand() {
    return Model::getUserTotal();
}

Card Model::doubleDown() {
    return Model::userHit();
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

int Model::getUserTotal() {
    return userTotal;
}

int Model::getDealerTotal() {
    return dealerTotal;
}

int Model::getbankTotal() {
    return bankTotal;
}

int Model::getBet() {
    return bet;
}

void Model::setBet(int increment) {
    if ((bankTotal - increment) >= 0) {
        bet += increment;
        bankTotal -= increment;
    }
}

void Model::resetBet() {
    bankTotal += bet;
    bet = 0;
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

void Model::split() {

}
