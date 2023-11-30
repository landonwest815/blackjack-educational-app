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

    /// @brief getDealerFaceUpValue used mainly to show a tip based on game state.
    /// @return returns the value of the dealers face up card.
    int getDealerFaceUpValue();

    /// \brief userHasAceInHand allows to check if ace in hand
    /// \return returns true or false depending if there are any aces at all.
    bool userHasAceInHand();

};

#endif // MODEL_H
