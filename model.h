#ifndef MODEL_H
#define MODEL_H

#include "deck.h"

class Model{
private:
    int bankTotal;
    int dealerTotal;
    int userTotal;

public:
    Model();

    int hit(int userTotal);

    int stand(Card first, Card second);

    int doubleDown(int userTotal);

    bool insurance(Card faceDown);

    int getUserTotal(Card first, Card second);

    int getDealerTotal(Card faceup, Card faceDown);

};

#endif // MODEL_H
