#include "model.h"
#include "deck.h"

#include <iostream>

Model::Model() :
    bankTotal(1500),
    dealerTotal(0),
    userTotal(0),
    bet(0),
    win(true),
    splitTotal(0),
    userAceCounter(0),
    dealerAceCounter(0),
    splitAceCounter(0),
    splitCheck(false){ }

Card Model::userHit() {
    Card nextCard = deck.drawCard();

    if(nextCard.getFace() == "A") {
        if(userTotal + nextCard.getValue() > 21) {
            userTotal += 1;
        } else {
            userTotal += 11;
            userAceCounter++;
        }
    } else {
        userTotal = userTotal + nextCard.getValue();
        if(userAceCounter > 0 && userTotal > 21) {
            userTotal -= 10;
            userAceCounter--;
        }
    }
    return nextCard;
}

Card Model::dealerHit(bool facedown) {
    Card nextCard = deck.drawCard();

    if(nextCard.getFace() == "A") {
        if(dealerTotal + nextCard.getValue() > 21) {
            dealerTotal += 1;
        } else {
            dealerTotal += 11;
            dealerAceCounter++;
        }
    } else {
        dealerTotal = dealerTotal + nextCard.getValue();
        if(dealerAceCounter > 0 && dealerTotal > 21) {
            dealerTotal -= 10;
            dealerAceCounter--;
        }
    }

    if (facedown) {
        dealerTotal -= nextCard.getValue();
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

bool Model::insurance() {
    Card nextCard = deck.drawCard();
    if(nextCard.getFace() == "A")
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

int Model::split() {
    userTotal = userTotal / 2;
    splitTotal = userTotal;
    return splitTotal;
}

int Model::getSplitTotal() {
    return splitTotal;
}

void Model::clearTotal() {
    userTotal = 0;
    dealerTotal = 0;
    splitTotal = 0;
}

Card Model::splitHit() {
    Card nextCard = deck.drawCard();

    if(nextCard.getFace() == "A") {
        if(splitTotal + nextCard.getValue() > 21) {
            splitTotal += 1;
        } else {
            splitTotal += 11;
            splitAceCounter++;
        }
    } else {
        splitTotal = splitTotal + nextCard.getValue();
        if(splitAceCounter > 0 && splitTotal > 21) {
            splitTotal -= 10;
            splitAceCounter--;
        }
    }
    return nextCard;
}

bool Model::getSplitCheck() {
    return splitCheck;
}

void Model::setSplitCheck(bool split ) {
    splitCheck = split;
}

Card Model::getUserCard(int index) {
    return userHand[index];
}

Card Model::getDealerCard(int index) {
    return dealerHand[index];
}

void Model::addUserCard(Card newCard) {
    userHand.push_back(newCard);
}

void Model::addDealerCard(Card newCard) {
    dealerHand.push_back(newCard);
}

void Model::revealDealer() {
    dealerTotal += dealerHand[0].getValue();
}

int Model::getDealerAces() {
    return dealerAceCounter;
}

void Model::playerBust() {
    dealerWins();
}

void Model::dealerBust() {
    playerWins();
}

void Model::playerWins() {
    win = true;
    resetBet();

    // Determine whether it's a blackjack win or a standard win
    if (getUserTotal() == 21 && userHand.size() == 2) {
        // Blackjack win
        userBlackJack(userHand[0], userHand[1]);
    } else {
        // Standard win
        updateBankTotal(getBet());
    }

    endRound();
}

void Model::dealerWins() {
    win = false;
    bet = 0;
    endRound();
}

void Model::handlePush() {
    resetBet();
    endRound();
}

void Model::endRound() {
    userHand.clear();
    dealerHand.clear();
}
