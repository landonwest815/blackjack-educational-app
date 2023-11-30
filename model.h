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
    vector<Card> userHand;
    vector<Card> dealerHand;

public:
    Model();

    Card userHit();

    Card dealerHit(bool facedown);

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

    void setSplitCheck(bool split);

    Card getUserCard(int index);

    Card getDealerCard(int index);

    void addUserCard(Card newCard);

    void addDealerCard(Card newCard);

    void revealDealer();

    int getDealerAces();

    void playerBust();

    void dealerBust();

    void playerWins();

    void dealerWins();

    void handlePush();

    void endRound();
};

#endif // MODEL_H
