#ifndef MODEL_H
#define MODEL_H

#include "card.h"

class Model{
private:
    int bankTotal;
    int dealerTotal;
    int userTotal;
    int bet;
    bool win;

public:
    Model();

    int hit(int userTotal);

    int stand(Card first, Card second);

    int doubleDown(int userTotal);

    bool insurance(Card faceDown);

    int getUserTotal(Card first, Card second);

    int getDealerTotal(Card faceup, Card faceDown);

    int getbankTotal();

    int updateBankTotal(int bet);

    void dealerBlackJack(Card facedown, Card faceUp);

    void userBlackJack(Card first, Card second);

};

#endif // MODEL_H
