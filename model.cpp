/***
 * Authors:         Team Six of Hearts
 * Members:         Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *                  Landon West, and Pablo Arancibia-Bazan
 * Course:          CS 3505, University of Utah, School of Computing
 * Assignment:      A9 - Educational App
 *
 * Description:     This class represents the core model of a Blackjack game. It manages
 *                  the deck, player and dealer hands, betting, and game state. The class
 *                  provides functionality for drawing cards, making bets, handling game
 *                  decisions like hit, stand, double down, and split, and determines
 *                  game outcomes.
 */

#include "model.h"
#include "deck.h"
#include <QDebug>

Model::Model()
    : bankTotal(1500), bet(0), playerTotal(0), dealerTotal(0),
    splitTotal(0), playerAceCounter(0), dealerAceCounter(0),
    splitAceCounter(0), win(true), splitCheck(false), onSecondHand(false) {}

Card Model::drawAndAddCard(int& total, int& aceCounter, bool faceDown) {

    // Draw a card and add it to the relative score
    Card nextCard = deck.drawCard();
    total += nextCard.getValue();

    // Check for Ace conditions
    if (nextCard.getFace() == "A") {
        aceCounter++;
        if (total > 21) {
            total -= 10;
            aceCounter--;
            nextCard.setValue(1);
        }
    } else if (aceCounter > 0 && total > 21) {
        total -= 10;
        aceCounter--;
    }

    // Check if card should be flipped down
    if (faceDown) {
        nextCard.setFaceDown(true);
        total -= nextCard.getValue();
    }

    return nextCard;
}

Card Model::userHit() {
    return drawAndAddCard(playerTotal, playerAceCounter, false);
}

Card Model::dealerHit(bool facedown) {
    return drawAndAddCard(dealerTotal, dealerAceCounter, facedown);
}

Card Model::splitHit() {
    return drawAndAddCard(splitTotal, splitAceCounter, false);
}

bool Model::shuffleCheck() {

    // Check if the deck is halfway depleted
    // We are using a 52 card deck as expected
    if (deck.getSize() < 27) {
        deck = Deck();
        return true;
    }

    // If no deck was created
    return false;
}

int Model::stand() {
    dealerAceCounter = 0;
    return playerTotal;
}

Card Model::doubleDown() {
    bankTotal -= bet;
    bet *= 2;
    return Model::userHit();
}

bool Model::insuranceAllowed() {
    return (dealerHand[1].getFace() == "A" && (bankTotal >= bet));
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
        playerTotal = 11;
        splitTotal = playerTotal;
    } else {
        playerTotal = playerTotal / 2;
        splitTotal = playerTotal;
    }
    splitCheck = true;
}

int Model::getSplitTotal() { return splitTotal; }

void Model::setOnSecondHand(bool secondHand) { onSecondHand = secondHand; }

bool Model::getOnSecondHand() { return onSecondHand; }

void Model::resetAllScores() {
    // Set all to zero
    playerTotal = dealerTotal = splitTotal = 0;
}

bool Model::getSplitCheck() { return splitCheck; }

void Model::setSplitCheck(bool split ) { splitCheck = split; }

Card Model::getUserCard(int index) { return userHand[index]; }

Card Model::getDealerCard(int index) { return dealerHand[index]; }

void Model::addUserCard(Card newCard) { userHand.push_back(newCard); }

void Model::addUserSplitCard(Card newCard) { userSplitHand.push_back(newCard); }

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
    updateBankAfterWin(bet);
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
    userSplitHand.clear();

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
    return (playerAceCounter > 0);
}

Card Model::getDealerFaceUpCard(){
    return dealerHand.at(1);
}

int Model::getTotalCardsForUser(){
    return (int)userHand.size();
}

void Model::setBlackJack(bool blackjack) {
    blackJack = blackjack;
}

bool Model::isBlackJack() {
    return blackJack;
}
