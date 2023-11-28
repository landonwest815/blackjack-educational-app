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

public:
    Model();

    Card hit();

    int stand(Card first, Card second);

    Card doubleDown();

    bool insurance(Card faceDown);

    int getUserTotal(Card first, Card second);

    int getDealerTotal(Card faceup, Card faceDown);

    int getbankTotal();

    int getBet();

    void setBet(int);

    void resetBet();

    int updateBankTotal(int bet);

    void dealerBlackJack(Card facedown, Card faceUp);

    void userBlackJack(Card first, Card second);

};

#endif // MODEL_H
