#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Contains all buttons so we can turn all of them off at once
    buttons = {ui->hitButton, ui->standButton, ui->splitButton, ui->nextSplitButton, ui->flipDealerButton, ui->doubleDemoButton,
               ui->add50, ui->add100, ui->add250, ui->add500, ui->resetButton, ui->dealButton, ui->insuranceButton, ui->nextHand,
               ui->allIn};

    // Contains all labels so we can turn all of them off at once
    labels = {ui->outcome, ui->splitScore};

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
    connect(ui->doubleDemoButton, &QPushButton::clicked, this, &MainWindow::doubleDownHand);

    // Menus
    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);
    connect(ui->quitGameMenu , &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);
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

        updateScores();

        // End round if player hits 21
        if (model.getUserTotal() >= 21 || model.getSplitTotal() >= 21) {
            if (!model.getSplitCheck()) {
                stand();
            }
            else {
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
    ui->playerHand->doubleDownPlayerCard(":/cards/AC.png");
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

    //Add dealer faceup card
//    addDealer();
    Card dealerFaceUp = model.dealerHit();

    QString faceUpFileName = QString::fromStdString(convertCardToPath(model.dealerHit()));

    ui->dealerHand->addDealerCard(faceUpFileName);
    ui->dealerScore->setText("DEALER SCORE: " + QString::number(model.getDealerTotal()));

    //get the dealers face up value and suit for giving tips
    dealerFaceUpValue = dealerFaceUp.getValue();
    dealerFaceUpSuit = dealerFaceUp.getFace();

    //Add dealer facedown card
    Card dealerFaceDown = model.dealerHit();
    dealerFaceDown.setFaceDown(true);
    QString faceDownFileName = QString::fromStdString(convertCardToPath(dealerFaceDown));

    ui->dealerHand->addDealerCard(faceDownFileName);
    ui->dealerScore->setText("DEALER SCORE: " + QString::number(model.getDealerTotal()));

    //Add player cards
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
    if (model.allowedToSplit()) ui->splitButton->setVisible(true);

    // Check for blackjack conditions
    if (model.getUserTotal() == 21) {
        if (!model.getSplitCheck()) {
            stand();
        }
        else {
            nextSplit();
        }
    }
}

void MainWindow::stand() {
    // must flip over facedown card
    dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
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

 void MainWindow::setupDeal() {
     // prompt the bet amounts
     hideAllUI();
     clearAll();
     model.endRound();

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

 void MainWindow::showATip(){
     int usersTotal = model.getUserTotal();
//     int dealersUpCard = figure this out
     // first we will go through the hard hands where ace is not counted in the count
     if(dealerFaceUpSuit != "A"){
        if(usersTotal == 5 ||
            usersTotal == 6 ||
            usersTotal == 7 ||
            usersTotal == 8){
            // add code to tell the user that hit is the best option
            if(dealerFaceUpValue){

            }
        }
        else if(usersTotal == 9 ||
                 usersTotal == 10 ||
                 usersTotal == 11){
            // check if the user can double down depending on if of what the
            // dealer has as the face up card
        }
        else if(usersTotal == 13 ||
                 usersTotal == 14 ||
                 usersTotal == 15 ||
                 usersTotal == 16 ||
                 usersTotal == 17){
            // add cod to say if 17, then just stand, but then also
            //check for cases for when the dealer has the upcard of either
            // 2 - 6
        }
     }
     else{
        // goes through all possible soft hand cases
        if(usersTotal == 13 ||
           usersTotal == 14 ||
           usersTotal == 15 ||
           usersTotal == 16 ||
           usersTotal == 17){
            // add code for if the user should hit or double down depending on face up value
        }
        else if(usersTotal == 18 ||
                 usersTotal == 19 ||
                 usersTotal == 20){
            // add code for if the user should stand or double down, and a special case of hit
        }
     }
 }
