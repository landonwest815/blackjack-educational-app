/***
 * Authors:         Team Six of Hearts
 * Members:         Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *                  Landon West, and Pablo Arancibia-Bazan
 * Course:          CS 3505, University of Utah, School of Computing
 * Assignment:      A9 - Educational App
 *
 * Description:     Header for the core model of a Blackjack game. It manages
 *                  the deck, player and dealer hands, betting, and game state. The class
 *                  provides functionality for drawing cards, making bets, handling game
 *                  decisions like hit, stand, double down, and split, and determines
 *                  game outcomes.
 */

#ifndef MODEL_H
#define MODEL_H

#include "card.h"
#include "deck.h"
#include <QString>
#include <QVector>

class Model{
private:

    ///@brief Deck of cards used in the game.
    Deck deck;

    ///@brief Total bank amount of the player.
    int bankTotal;
    ///@brief Current bet amount.
    int bet;

    ///@brief Total value of player's hand.
    int playerTotal;
    ///@brief Total value of dealer's hand.
    int dealerTotal;
    ///@brief Total value of player's split hand.
    int splitTotal;

    ///@brief Counts the number of aces in the player's hand.
    int playerAceCounter;
    ///@brief Counts the number of aces in the dealer's hand.
    int dealerAceCounter;
    ///@brief Counts the number of aces in the player's split hand.
    int splitAceCounter;

    ///@brief Indicates if the player won the last round.
    bool win;
    ///@brief Indicates if the player or dealer has a blackjack.
    bool blackJack;

    ///@brief Indicates if the player has split their hand.
    bool splitCheck;
    ///@brief Indicates if the player is on their second hand in a split.
    bool onSecondHand;

    ///@brief Cards in the player's hand.
    QVector<Card> userHand;
    ///@brief Cards in the dealer's hand.
    QVector<Card> dealerHand;
    ///@brief Cards in the player's split hand.
    QVector<Card> userSplitHand;

public:
    ///@brief Constructor for the Model class. (empty)
    Model();

    // USER ACTIONS
    ///@brief Player hits and gets another card.
    Card userHit();
    ///@brief Dealer hits and gets another card. Can be facedown.
    Card dealerHit(bool facedown);
    ///@brief Player hits on a split hand.
    Card splitHit();
    ///@brief Returns player score. Player stands and ends their turn.
    int stand();
    ///@brief Player doubles down their bet.
    Card doubleDown();
    ///@brief Handles the splitting of the user's hand.
    void split();

    // SCORES
    ///@brief Returns the total value of the player's hand.
    int getUserTotal();
    ///@brief Returns the total value of the dealer's hand.
    int getDealerTotal();
    ///@brief Returns the total value of the players's split hand.
    int getSplitTotal();
    ///@brief Resets all scores for a new game.
    void resetAllScores();

    // BANKING
    ///@brief Returns the total value of the bank.
    int getbankTotal();
    ///@brief Adjusts the bank total with the int parameter.
    void adjustBankTotal(int);
    ///@brief Returns the current bet amount.
    int getBet();
    ///@brief Sets the bet amount.
    void setBet(int);
    ///@brief Resets the bet amount.
    void resetBet();
    ///@brief Updates bank balance after a win.
    int updateBankAfterWin(int bet);

    // CONDITIONS GETTERS & SETTERS
    ///@brief Checks if insurance is allowed.
    bool insuranceAllowed();
    ///@brief Handles dealer blackjack.
    void dealerBlackJack(Card facedown, Card faceUp);
    ///@brief Handles player blackjack.
    void userBlackJack(Card first, Card second);
    ///@brief Checks if the user is allowed to split their hand.
    bool allowedToSplit();
    ///@brief Returns whether the user has split their hand.
    bool getSplitCheck();
    ///@brief Sets whether the player has split their hand.
    void setSplitCheck(bool split);
    ///@brief Returns whether the player is on their second split hand.
    bool getOnSecondHand();
    ///@brief Sets whether the player is on their second split hand.
    void setOnSecondHand(bool);
    ///@brief Checks if the user has an ace in their hand.
    bool userHasAceInHand();
    ///@brief Returns the dealer's face-up card.
    Card getDealerFaceUpCard();
    ///@brief Returns the total number of cards in the user's hand.
    int getTotalCardsForUser();
    ///@brief Returns the value of the dealer's facedown card.
    int faceDownValue();
    ///@brief Sets the blackjack status.
    void setBlackJack(bool);
    ///@brief Returns whether there is a blackjack.
    bool isBlackJack();

    // CARDS
    ///@brief Returns a specific card from the user's hand.
    Card getUserCard(int index);
    ///@brief Returns a specific card from the dealer's hand.
    Card getDealerCard(int index);
    ///@brief Adds a card to the user's hand.
    void addUserCard(Card newCard);
    ///@brief Adds a card to the dealer's hand.
    void addDealerCard(Card newCard);
    ///@brief Adds a card to the user's split hand.
    void addUserSplitCard(Card newCard);
    ///@brief Reveals the dealer's facedown card.
    Card revealDealer();
    ///@brief Returns the number of aces in the dealer's hand.
    int getDealerAces();
    ///@brief Draws a card for the relative hand
    Card drawAndAddCard(int& total, int& aceCounter, bool faceDown);

    // GAME STATES
    ///@brief Handles player winning.
    void playerWins();
    ///@brief Handles dealer winning.
    void dealerWins();
    ///@brief Handles player and dealer tying.
    void handlePush();
    ///@brief Ends the current round.
    void endRound();
    ///@brief Checks if the deck needs shuffling.
    bool shuffleCheck();
};

#endif // MODEL_H
