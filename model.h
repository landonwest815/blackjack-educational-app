#ifndef MODEL_H
#define MODEL_H

#include "card.h"
#include "deck.h"
#include <QString>

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

public:
    Model();

    Card userHit();

    Card dealerHit();

    Card splitHit();

    int stand();

    Card doubleDown();

    bool insurance();

    int getUserTotal();

    int getDealerTotal();

    int getbankTotal();

    int getBet();

    void setBet(int);

    void resetBet();

    int updateBankTotal(int bet);

    void dealerBlackJack(Card facedown, Card faceUp);

    void userBlackJack(Card first, Card second);

    int split ();

    int getSplitTotal();

    void clearTotal();

    bool getSplitCheck();

    bool setSplitCheck(bool split);
};

#endif // MODEL_H
