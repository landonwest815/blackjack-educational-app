#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tutorialStep(1)
{
    ui->setupUi(this);

    // Contains all buttons so we can turn all of them off at once
    buttons = {ui->hitButton, ui->standButton, ui->splitButton, ui->nextSplitButton, ui->flipDealerButton, ui->doubleDemoButton,
               ui->add50, ui->add100, ui->add250, ui->add500, ui->resetButton, ui->dealButton, ui->insuranceButton, ui->nextHand,
               ui->allIn, ui->nextStepOneButton, ui->nextStepTwoButton, ui->nextStepThreeButton, ui->adviceButton};

    // Contains all labels so we can turn all of them off at once
    labels = {ui->outcome, ui->splitScore, ui->tutorialLabel};

    setupConnections();
    initializeUI();
    //createHelpWidget("This is a very helpful tip on how to win the game!! blah blah blah blah blah blah. Is this a sentence?");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {

    // Turn Actions
    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::addPlayer);
    connect(ui->standButton, &QPushButton::clicked, this, &MainWindow::stand);

    // Splitting
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::splitHand);
    connect(ui->nextSplitButton, &QPushButton::clicked, this, &MainWindow::nextSplit);

    // End of Turn
    connect(ui->nextHand, &QPushButton::clicked, this, &MainWindow::setupDeal);

    // Betting
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });
    connect(ui->allIn, &QPushButton::clicked, this, [this](){ addToBet(model.getbankTotal()); });
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);
    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::deal);
   // connect(ui->doubleDemoButton, &QPushButton::clicked, this, &MainWindow::doubleDownHand);
    connect(ui->insuranceButton, &QPushButton::clicked, this, &MainWindow::insurance);
    connect(ui->doubleDemoButton, &QPushButton::clicked, this, &MainWindow::doubleDown);

    // Menus
    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->mainMenuButton, &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);
    connect(ui->tutorial, &QPushButton::clicked, this, &MainWindow::switchToLessonsWindow);


    connect(ui->lessonOneButton, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);
    connect(ui->lessonTwoButton, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);
    connect(ui->lessonThreeButton, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);

    connect(ui->nextStepOneButton, &QPushButton::clicked, this, &MainWindow::nextLessonOneStep);
    connect(ui->nextStepTwoButton, &QPushButton::clicked, this, &MainWindow::nextLessonTwoStep);
    connect(ui->nextStepThreeButton, &QPushButton::clicked, this, &MainWindow::nextLessonThreeStep);

    connect(ui->quitGameMenu, &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);
    connect(ui->adviceButton, &QPushButton::clicked, this, &MainWindow::displayAdvice);
}

void MainWindow::initializeUI() {

    // Set the screen to the Main Menu
    ui->stackedWidget->setCurrentWidget(ui->startMenu);

    // Initially hide all buttons and labels
    hideAllUI();

    updateBankDisplay();
}

void MainWindow::hideAllUI() {
    for (QPushButton* button : buttons) {
        button->setVisible(false);
    }
    for (QLabel* label : labels) {
        label->setVisible(false);
    }
}

void MainWindow::updateBankDisplay() {
    ui->bank->setText("BANK: $" + QString::number(model.getbankTotal()));
    ui->currentBet->setText("BET: $" + QString::number(model.getBet()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    //if (helpwidget) helpwidget->setSize();
}

void MainWindow::createHelpWidget(QString text) {
    helpwidget = new HelpWidget(this);
    helpwidget->setText(text);
    helpwidget->show();
}

// Just example code for how you would call to add a card or clear them from the Box2D scene
void MainWindow::addDealer(bool facedown) {

    // draw a card
    Card dealerCard = model.dealerHit(facedown);
    model.addDealerCard(dealerCard);

    ui->dealerHand->addDealerCard(QString::fromStdString(convertCardToPath(dealerCard)));
    updateScores();
}

void MainWindow::addPlayer() {

    if (!model.getOnSecondHand()) {
        Card playerCard = model.userHit();
        model.addUserCard(playerCard);

        ui->playerHand->addPlayerCard(QString::fromStdString(convertCardToPath(playerCard)));
    }
    else {
        Card playerCard = model.splitHit();
        model.addUserCard(playerCard);

        ui->playerHand->addPlayerCard(QString::fromStdString(convertCardToPath(playerCard)));
    }

        ui->insuranceButton->setVisible(false);
        updateScores();

        // End round if player hits 21
        if (model.getUserTotal() > 21 || model.getSplitTotal() > 21) {
            if (!model.getSplitCheck()) {
                    dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
                    updateScores();
                    determineWinner();
            } else if(model.getUserTotal() == 21 || model.getSplitTotal() == 21) {
                    while(model.getDealerTotal() < 17) {
                        addDealer(true);
                        updateScores();
                    }
                    determineWinner();
            } else {
                nextSplit();
            }
        }

        qDebug() << model.getOnSecondHand();

}

void MainWindow::splitHand() {
    ui->playerHand->splitPlayerCards();
    ui->splitScore->setVisible(true);
    ui->nextSplitButton->setVisible(true);
    ui->splitButton->setVisible(false);
    model.split();
    updateScores();
}

void MainWindow::nextSplit() {
    ui->playerHand->nextSplitHand();
    model.setOnSecondHand(true);
    model.setSplitCheck(false);
}

void MainWindow::dealerFlip(QString fileName) {
    ui->dealerHand->flipDealerCard(fileName);
}

void MainWindow::doubleDownHand() {
    doubleDown();
}

void MainWindow::doubleDown() {
    Card doubleDownCard = model.userHit();
    model.addUserCard(doubleDownCard);
    ui->playerHand->doubleDownPlayerCard(QString::fromStdString(convertCardToPath(doubleDownCard)));
    addToBet(2 * model.getBet());
    stand();
}

string MainWindow::convertCardToPath(Card card) {
    string suit = card.getSuit();
    int value = card.getValue();
    string face = card.getFace();

    string path = "";
    if (card.getFaceDown()) {
        path = ":/cards/facedown.png";
    } else if (face == "") {
        path = ":/cards/" + std::to_string(value) + suit + ".png";
    } else {
        path = ":/cards/" + face + suit + ".png";
    }

    return path;
}

void MainWindow::clearAll() {
    ui->playerHand->clearAllCards();
    ui->dealerHand->clearAllCards();
    ui->playerScore->setText("PLAYER SCORE: " + QString::number(0));
    ui->dealerScore->setText("DEALER SCORE: " + QString::number(0));
    ui->splitScore->setText("SPLIT SCORE: " + QString::number(0));
    model.clearTotal();
    model.setSplitCheck(false);
}

void MainWindow::addToBet(int increment) {
    model.setBet(increment);
    updateBankDisplay();
}

void MainWindow::resetBet() {
    model.resetBet();
    updateBankDisplay();
}

void MainWindow::deal() {

    // Deal to player
    addPlayer();
    addPlayer();

    // Deal to dealer
    addDealer(true);
    addDealer(false);

    // Facedown card should not be included in score
    updateScores();

    // Update the UI
    hideAllUI();
    ui->hitButton->setVisible(true);
    ui->standButton->setVisible(true);
    ui->doubleDemoButton->setVisible(true);
    if (model.allowedToSplit()) ui->splitButton->setVisible(true);

    // Check for blackjack conditions
    if (model.getUserTotal() == 21) {
        dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
        updateScores();
        determineWinner();
    }

    if(model.getDealerTotal() == 10) {
        updateScores();
        if(model.getDealerTotal() == 21) {
            dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
                determineWinner();
        }
    }
    if(model.insuranceAllowed()) {
        ui->insuranceButton->setVisible(true);
    }
}

void MainWindow::stand() {
    // must flip over facedown card
    dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
    ui->insuranceButton->setVisible(false);
    updateScores();

    while (model.getDealerTotal() < 17 || (model.getDealerTotal() == 17 && model.getDealerAces() > 0)) {
        addDealer(false);
    }

    determineWinner();
}

void MainWindow::determineWinner() {
    int userTotal = model.getUserTotal();
    int dealerTotal = model.getDealerTotal();

    // Check for bust conditions
    if (userTotal > 21) {
        model.playerBust();
        showOutcome("PLAYER BUSTS!");

    } else if (dealerTotal > 21) {
        model.dealerBust();
        showOutcome("DEALER BUSTS!");

    } else {
        // Compare hand values to determine the winner
        if (userTotal > dealerTotal) {
            model.playerWins();
            showOutcome("PLAYER WINS!");

        } else if (userTotal < dealerTotal) {
            model.dealerWins();
            showOutcome("DEALER WINS!");

        } else {
            model.handlePush(); // when a tie occurs
            model.dealerWins();
            showOutcome("PUSH!");
        }
    }

    updateBankDisplay();
}

void MainWindow::showOutcome(QString outcome) {
    hideAllUI();
    ui->outcome->setVisible(true);
    ui->outcome->setText(outcome);
    ui->nextHand->setVisible(true);
}

 void MainWindow::onQuitGameClicked()
 {
     this->close();
 }

 void MainWindow::switchToMainMenu() {
     ui->stackedWidget->setCurrentWidget(ui->startMenu);
     tutorialStep = 1;
     ui->dealerScore->setVisible(true);
     ui->playerScore->setVisible(true);
     ui->bank->setVisible(true);
     ui->currentBet->setVisible(true);
 }

 void MainWindow::updateScores() {
     ui->dealerScore->setText("DEALER SCORE: " + QString::number(model.getDealerTotal()));
     ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getUserTotal()));
     ui->splitScore->setText("SPLIT SCORE: " + QString::number(model.getSplitTotal()));
 }

 void MainWindow::switchToGameWindow() {
     ui->stackedWidget->setCurrentWidget(ui->game);
     setupDeal();
 }

 void MainWindow::switchToLessonsWindow() {
     ui->stackedWidget->setCurrentWidget(ui->lessons);
 }

 void MainWindow::setupDeal() {
     // prompt the bet amounts
     hideAllUI();
     clearAll();
     model.endRound();
     addToBet(100);

     // Show Betting Actions
     ui->add50->setVisible(true);
     ui->add100->setVisible(true);
     ui->add250->setVisible(true);
     ui->add500->setVisible(true);
     ui->allIn->setVisible(true);
     ui->resetButton->setVisible(true);
     ui->dealButton->setVisible(true);
 }

 void MainWindow::splitAdd() {
     QString fileName = QString::fromStdString(convertCardToPath(model.splitHit()));

     ui->playerHand->addPlayerCard(fileName);
     updateScores();
 }

 void MainWindow::insurance() {
     int sideBet = model.getBet() / 2 ;

     if(model.getDealerTotal() + model.faceDownValue() == 21) {
        model.adjustBankTotal(2 * sideBet);
     } else {
        model.adjustBankTotal(-sideBet);
     }

     ui->insuranceButton->setVisible(false);
     updateBankDisplay();
 }

 void MainWindow::handleLessonSelect() {
     ui->dealerHand->clearAllCards();
     ui->playerHand->clearAllCards();
     ui->stackedWidget->setCurrentWidget(ui->game);
     hideAllUI();
     ui->dealerScore->setVisible(false);
     ui->playerScore->setVisible(false);
     ui->bank->setVisible(false);
     ui->currentBet->setVisible(false);
     ui->tutorialLabel->setVisible(true);
     if (sender() == ui->lessonOneButton) {
        ui->nextStepOneButton->setVisible(true);
     }
     if (sender() == ui->lessonTwoButton) {
        ui->nextStepTwoButton->setVisible(true);
     }
     if (sender() == ui->lessonThreeButton) {
        ui->nextStepThreeButton->setVisible(true);
     }
     ui->playerHand->enableTextBox();
     ui->playerHand->setTextBox("ℹ️ Tutorial Selected\n"
                                "To progess through the tutorial please click the\n"
                                "'Next Step' button.");
 }

 void MainWindow::nextLessonOneStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->dealerHand->addDealerCard(":/cards/7D.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/QC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Card Values\n"
                                   "1) Number cards (2-10) are worth their face value.\n"
                                   "2) Face cards (Jack, Queen, King) are each worth 10.\n"
                                   "3) Aces can be worth 1 or 11, whichever is more beneficial.\n"
                                   "🤓 Example\n"
                                   "Dealer Hand) 7D(7) + 10C(10) = 17\n"
                                   "Player Hand) QC(10) + AC(1/11) = 11 or 21");
        break;
     case 2:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->addPlayerCard(":/cards/5S.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Game Setup\n"
                                   "1) Each player, including the dealer, is dealt two cards.\n"
                                   "Players' cards are usually face-up.\n"
                                   "2) While one of the dealer's cards is face-up (known as the\n"
                                   "upcard) and the other face-down (the hole card).");
        break;
     case 3:
        ui->playerHand->addPlayerCard(":/cards/3H.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Player Turns\n"
                                   "1) Players decide whether to 'hit' (take another card) or\n"
                                   "'stand' (keep their current hand).\n"
                                   "2) Players can continue to hit as many times as they want,\n"
                                   "but if the total value of their hand exceeds 21, they 'bust'\n"
                                   "and lose the game.");
        break;
     case 4:
        ui->dealerHand->flipDealerCard(":/cards/7C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Dealer Turns\n"
                                   "1) The dealer reveals their hole card.\n"
                                   "2) The dealer must draw on 16 or under and must stand on 17\n"
                                   "or over.");
        break;
     case 5:
        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Winning\n"
                                   "1) If a player's hand is closer to 21 than the dealer's\n"
                                   "without busting, the player wins.\n"
                                   "2) If the dealer busts, all remaining players win.\n"
                                   "3) If both the player and dealer have the same hand value, it's\n"
                                   "a 'push' (a tie), and the player's bet is returned.");
        break;
     case 6:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/KC.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Special Moves\n"
                                   "♦️ Blackjack\n"
                                   "If a player is dealt an Ace and a 10-value card as their initial\n"
                                   "two cards, it's called a 'Blackjack' or 'natural' and the player\n"
                                   "wins unless dealer also has Blackjack.");
        break;
     case 7:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/3D.png");
        ui->playerHand->splitPlayerCards();

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Special Moves\n"
                                   "♥️ Split\n"
                                   "If a player's initial two cards are of the same value, they can\n"
                                   "choose to split them into two separate hands, each with its bet.");
        break;
     case 8:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/8D.png");
        ui->playerHand->doubleDownPlayerCard(":/cards/QC.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Special Moves\n"
                                   "♠️ Double Down\n"
                                   "After receiving the first two cards, a player can choose\n"
                                   "to double their original bet in exchange for committing\n"
                                   "to stand after receiving one more card.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();

        ui->nextStepOneButton->setVisible(false);
        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ 'Rules of Blackjack' Tutorial Complete\n"
                                   "Please click the 'Main Menu' button to return back\n"
                                   "to the home screen.");
        return;
     }
     // Increment the tutorial step
     ++tutorialStep;
 }



 void MainWindow::nextLessonTwoStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->playerHand->addPlayerCard(":/cards/KC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->splitPlayerCards();
        ui->playerHand->addPlayerCard(":/cards/7C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Hit or Stand\n"
                                   "1) If your hand is at a total of 17 or higher,\n"
                                   "it's generally advisable to stand to avoid the\n"
                                   "risk of busting.\n"
                                   "2) When your hand is below a certain total\n"
                                   "(usually 11), it is often recommended to hit\n"
                                   "to improve your hand.");
        break;
     case 2:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/6H.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->doubleDownPlayerCard(":/cards/AC.png");
        ui->dealerHand->addDealerCard(":/cards/QH.png");
        ui->dealerHand->addDealerCard(":/cards/6C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Doubling Down\n"
                                   "Double down when you have a hand value of\n"
                                   "10 or 11, especially when the dealer's face-up\n"
                                   "card is weaker.");
        break;
     case 3:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->splitPlayerCards();

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Splitting Pairs\n"
                                   "1) Split if you get a pair of Aces.\n"
                                   "2) Split if you get a pair of 8s.\n"
                                   "3) Avoid spitting 10s, value of 20 is strong.");
        break;
     case 4:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/JC.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Insurance\n"
                                   "Avoid taking insurance bets, as they statistically favor\n"
                                   "the house in the long run.");
        break;
     case 5:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Soft Hands\n"
                                   "With a soft hand (an Ace and another card), it's often\n"
                                   "safe to hit without the risk of busting since Ace is 1 or 11.");
        break;
     case 6:
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/AD.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Dealer's Up Card\n"
                                   "Pay attention to the dealer's up card. Adjust your\n"
                                   "strategy based on their visible card, as it gives insight\n"
                                   "into the likelihood of their final hand.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();

        ui->nextStepTwoButton->setVisible(false);
        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ 'Basic Strategies' Tutorial Complete\n"
                                   "Please click the 'Main Menu' button to return back\n"
                                   "to the home screen.");
        return;
     }
     // Increment the tutorial step
     ++tutorialStep;
 }


 void MainWindow::nextLessonThreeStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->playerHand->addPlayerCard(":/cards/1C.png");
        ui->playerHand->addPlayerCard(":/cards/2C.png");
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->addPlayerCard(":/cards/5C.png");
        ui->playerHand->addPlayerCard(":/cards/6C.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");
        ui->playerHand->addPlayerCard(":/cards/8C.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");
        ui->playerHand->addPlayerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Card Counting\n"
                                   "Keep track of the ratio of high to low-value cards\n"
                                   "remaining in the deck. Adjust your bets and actions\n"
                                   "accordingly\n"
                                   "🤓 Example\n"
                                   "Use the Hi-Lo counting system where cards 2-6 have\n"
                                   "a value of +1, 7-9 have a value of 0, and 10-Ace have\n"
                                   "a value of -1. Keep a running count as the cards are dealt.");
        break;
     case 2:
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");
        ui->playerHand->addPlayerCard(":/cards/4H.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Basic Strategy Variations\n"
                                   "Depending on the specific rules of the game, there\n"
                                   "might be variations in basic strategy. Learn and apply\n"
                                   "these variations for optimal play.\n"
                                   "🤓 Example\n"
                                   "Modify basic strategy based on the count. For a positive\n"
                                   "count, consider doubling down on 11 against a dealer's 10,\n"
                                   "even if basic strategy suggests otherwise.");
        break;
     case 3:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/AH.png");
        ui->dealerHand->addDealerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AS.png");
        ui->playerHand->addPlayerCard(":/cards/2H.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Side Counting\n"
                                   "Track specific cards, such as Aces, separately from the\n"
                                   "overall count to gain a more accurate picture of the deck\n"
                                   "composition.\n"
                                   "🤓 Example\n"
                                   "Look at what cards are on the table like in this example\n"
                                   "three aces are out at play.");
        break;
     case 4:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->dealerHand->addPlayerCard(":/cards/AH.png");
        ui->dealerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/AS.png");

        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ Beware\n"
                                   "It's essential to note that while basic strategies can\n"
                                   "improve your game, advanced strategies like card counting\n"
                                   "may be restricted or disallowed by different instituions,\n"
                                   "and attempting them could lead to being barred from play.\n"
                                   "Always be aware of and adhere to different rules and\n"
                                   "regulations.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();

        ui->nextStepThreeButton->setVisible(false);
        ui->playerHand->enableTextBox();
        ui->playerHand->setTextBox("ℹ️ 'Advanced Strategies' Tutorial Complete\n"
                                   "Please click the 'Main Menu' button to return back\n"
                                   "to the home screen.");
        return;
     }
     // Increment the tutorial step
     ++tutorialStep;
 }



 void MainWindow::displayAdvice() {

 }

