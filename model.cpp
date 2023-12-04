#include "model.h"
#include "deck.h"

#include <iostream>
#include <QDebug>

Model::Model() :
    bankTotal(1500),
    dealerTotal(0),
    playerTotal(0),
    bet(0),
    win(true),
    splitTotal(0),
    playerAceCounter(0),
    dealerAceCounter(0),
    splitAceCounter(0),
    splitCheck(false),
    onSecondHand(false){ }

Card Model::userHit() {

    // Draw a card from deck
    Card nextCard = deck.drawCard();

    // If an Ace is drawn adjust the value if beneficial
    if(nextCard.getFace() == "A") {
        playerTotal += 11;
        playerAceCounter++;
        if(playerTotal > 21) {
            playerTotal -= 10;
            playerAceCounter--;
        }
    } else {
        playerTotal += nextCard.getValue();
        if(playerAceCounter > 0 && playerTotal > 21) {
            playerTotal -= 10;
            playerAceCounter--;
        }
    }
    return nextCard;
}

Card Model::dealerHit(bool facedown) {

    // Draw a card from deck
    Card nextCard = deck.drawCard();

    // If an Ace is drawn adjust the value if beneficial
    if(nextCard.getFace() == "A") {
        dealerTotal += 11;
        dealerAceCounter++;
        if(dealerTotal > 21) {
            dealerTotal -= 10;
            dealerAceCounter--;
        }
    } else {
        dealerTotal = dealerTotal + nextCard.getValue();
        if(dealerAceCounter != 0 && dealerTotal > 21) {
            dealerTotal -= 10;
            dealerAceCounter--;
        }
    }

    // If this is the face down card subtract the value to keep the dealer total hidden
    if (facedown) {
        dealerTotal -= nextCard.getValue();
        nextCard.setFaceDown(true);
    }
    return nextCard;
}

Card Model::splitHit() {

    // Draw a card from deck
    Card nextCard = deck.drawCard();

    // If an Ace is drawn adjust the value if beneficial
    if(nextCard.getFace() == "A") {
        splitTotal += 11;
        splitAceCounter++;
        if(splitTotal > 21) {
            splitTotal -= 10;
            splitAceCounter--;
        }
    } else {
        splitTotal += nextCard.getValue();
        if(splitAceCounter > 0 && splitTotal > 21) {
            splitTotal -= 10;
            splitAceCounter--;
        }
    }
    return nextCard;
}

bool Model::shuffleCheck() {

    // Check if the deck is halfway depleted
    // We are using a 52 card deck as expected
    if (deck.getSize() < 27) {
        Deck newDeck;
        deck = newDeck;
        return true;
    }
    else {
        return false;
    }
}

int Model::stand() {
    return Model::getUserTotal();
    dealerAceCounter = 0;
}

Card Model::doubleDown() {
    return Model::userHit();
    bankTotal -= bet;
    bet *= 2;
}

bool Model::insuranceAllowed() {
    // If the dealer's second card is an Ace (the first card is the face down card)
    return (dealerHand[1].getFace() == "A");
}

int Model::getUserTotal() { return playerTotal; }

int Model::getDealerTotal() { return dealerTotal; }

int Model::getbankTotal() { return bankTotal; }

void Model::adjustBankTotal(int total) { bankTotal += total; }

int Model::getBet() { return bet; }

void Model::setBet(int increment) {
    if ((bankTotal - increment) >= 0) {
        bet += increment;
        bankTotal -= increment;
    } else {
        bet += bankTotal;
        bankTotal = 0;
    }
}

void Model::resetBet() {
    bankTotal += bet - 100;
    bet = 100;
}

int Model::updateBankAfterWin(int bet) {
    if(win) bankTotal += 2 * bet;

    return bankTotal;
}

void Model::userBlackJack(Card first, Card second) {
    if(first.getValue() + second.getValue() == 21)
        this->bankTotal += (2 * this->bet * 1.5);
}

void Model::dealerBlackJack(Card faceDown, Card faceUp) {
    if(faceDown.getValue() + faceUp.getValue() == 21)
        this->bankTotal -= this->bet;
}

bool Model::allowedToSplit() {
    return (userHand[0].getValue() == userHand[1].getValue());
}

void Model::split() {
    if(userHand[0].getFace() == "A" && userHand[1].getFace() == "A") {
        userTotal = 11;
        splitTotal = userTotal;
    } else {
        userTotal = userTotal / 2;
        splitTotal = userTotal;
    }
    splitCheck = true;
}

int Model::getSplitTotal() { return splitTotal; }

void Model::setOnSecondHand(bool secondHand) { onSecondHand = secondHand; }

bool Model::getOnSecondHand() { return onSecondHand; }

void Model::resetAllScores() {
    playerTotal = 0;
    dealerTotal = 0;
    splitTotal = 0;
}

bool Model::getSplitCheck() { return splitCheck; }

void Model::setSplitCheck(bool split ) { splitCheck = split; }

Card Model::getUserCard(int index) { return userHand[index]; }

Card Model::getDealerCard(int index) { return dealerHand[index]; }

void Model::addUserCard(Card newCard) { userHand.push_back(newCard); }

void Model::addDealerCard(Card newCard) { dealerHand.push_back(newCard); }

Card Model::revealDealer() {
    dealerTotal += faceDownValue();
    dealerHand[0].setFaceDown(false);
    return dealerHand[0];
}

int Model::faceDownValue() { return dealerHand[0].getValue(); }

int Model::getDealerAces() { return dealerAceCounter; }

void Model::playerWins() {
    win = true;

    // Determine whether it's a blackjack win or a standard win
    if (getUserTotal() == 21 && userHand.size() == 2) {
        // Blackjack win
        userBlackJack(userHand[0], userHand[1]);
    } else {
        // Standard win
        updateBankAfterWin(bet);
    }
}

void Model::dealerWins() {
    win = false;
}

void Model::handlePush() {
    bankTotal += bet;
}

void Model::endRound() {

    // Clear all hand data
    userHand.clear();
    dealerHand.clear();

    // Clear all Ace data
    playerAceCounter = 0;
    dealerAceCounter = 0;
    splitAceCounter = 0;

    // Reset split data
    onSecondHand = false;
    splitCheck = false;

    // Empty the bet
    bet = 0;
}

bool Model::userHasAceInHand() {
    for(auto card : userHand) {
        if(card.getFace() == "A") {
            return true;
        }
    }

    // Better?
    // return (aceCounter != 0)

    return false;
}

Card Model::getDealerFaceUpCard(){
    return dealerHand.at(1);
}

int Model::getTotalCardsForUser(){
    return (int)userHand.size();
}
