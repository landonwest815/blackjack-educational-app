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
    int userTotal;
    int bet;
    bool win;
    int splitTotal;
    int userAceCounter;
    int dealerAceCounter;
    int splitAceCounter;
    bool splitCheck;
    bool onSecondHand;
    vector<Card> userHand;
    vector<Card> dealerHand;

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

    int updateBankTotal(int bet);

    void dealerBlackJack(Card facedown, Card faceUp);

    void userBlackJack(Card first, Card second);

    bool allowedToSplit();

    void split ();

    int getSplitTotal();

    void clearTotal();

    bool getSplitCheck();

    void setSplitCheck(bool split);

    Card getUserCard(int index);

    Card getDealerCard(int index);

    void addUserCard(Card newCard);

    void addDealerCard(Card newCard);

    Card revealDealer();

    int getDealerAces();

    void playerBust();

    void dealerBust();

    void playerWins();

    void dealerWins();

    void handlePush();

    void endRound();

    void shuffleCheck();

    bool getOnSecondHand();

    void setOnSecondHand(bool);

    int faceDownValue();

};

#endif // MODEL_H
