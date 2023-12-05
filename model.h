#ifndef MODEL_H
#define MODEL_H

#include "card.h"
#include "deck.h"
#include <QString>
#include <vector>

class Model{
private:
    Deck deck;
    int bankTotal;
    int startingCash;
    int dealerTotal;
    int playerTotal;
    int bet;
    bool win;
    int splitTotal;
    int playerAceCounter;
    int dealerAceCounter;
    int splitAceCounter;
    bool splitCheck;
    bool onSecondHand;
    vector<Card> userHand;
    vector<Card> dealerHand;
    vector<Card> userSplitHand;

public:
    Model();

    Card userHit();

    Card dealerHit(bool facedown);

    Card splitHit();

    int stand();

    Card doubleDown();

    bool insuranceAllowed();

    int getUserTotal();

    int getDealerTotal();

    int getbankTotal();

    void adjustBankTotal(int);

    int getBet();

    void setBet(int);

    void resetBet();
    
    int updateBankAfterWin(int bet);

    void dealerBlackJack(Card facedown, Card faceUp);

    void userBlackJack(Card first, Card second);

    bool allowedToSplit();

    void split ();

    int getSplitTotal();
    
    void resetAllScores();

    bool getSplitCheck();

    void setSplitCheck(bool split);

    Card getUserCard(int index);

    Card getDealerCard(int index);

    void addUserCard(Card newCard);

    void addDealerCard(Card newCard);

    void addUserSplitCard(Card newCard);

    Card revealDealer();

    int getDealerAces();

    void playerWins();

    void dealerWins();

    void handlePush();

    void endRound();

    bool shuffleCheck();

    bool getOnSecondHand();

    void setOnSecondHand(bool);

    /// \brief userHasAceInHand allows to check if ace in hand
    /// \return returns true or false depending if there are any aces at all.
    bool userHasAceInHand();

    /// \brief getDealerFaceUpFace used to get information of face up card
    /// \return returns the face up card of the dealer
    Card getDealerFaceUpCard();

    int getTotalCardsForUser();
    int faceDownValue();

};

#endif // MODEL_H
