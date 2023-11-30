#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();
    initializeUI();
    //createHelpWidget("This is a very helpful tip on how to win the game!! blah blah blah blah blah blah. Is this a sentence?");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {

    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::splitHand);
    connect(ui->standButton, &QPushButton::clicked, this, &MainWindow::stand);

    connect(ui->nextSplitButton, &QPushButton::clicked, this, &MainWindow::nextSplit);

    // Bet buttons
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });

    // Reset Bet buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);

    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);

    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);

    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::beginGame);

    connect(ui->quitGameMenu , &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);

    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::addPlayer);

}

void MainWindow::initializeUI() {

    ui->stackedWidget->setCurrentWidget(ui->startMenu);

    toggleBetButtons(false);
    ui->dealButton->setVisible(false);
    ui->resetButton->setVisible(false);
    buttonState = true;

    ui->splitScore->setVisible(false);

    updateBankDisplay();
}

void MainWindow::toggleBetButtons(bool visible) {
    QList<QPushButton*> buttons = {ui->add50, ui->add100, ui->add250, ui->add500};
    for (QPushButton *button : buttons) {
        button->setVisible(visible);
    }
}

void MainWindow::updateBankDisplay() {
    ui->bank->setText("BANK: $" + QString::number(model.getbankTotal()));
    ui->currentBet->setText("CURRENT BET: $" + QString::number(model.getBet()));
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
void MainWindow::addDealer() {
    QString fileName = QString::fromStdString(convertCardToPath(model.dealerHit()));

    ui->dealerHand->addDealerCard(fileName);
    ui->dealerScore->setText("DEALER SCORE: " + QString::number(model.getDealerTotal()));
}

void MainWindow::addPlayer() {
    QString fileName = QString::fromStdString(convertCardToPath(model.userHit()));

    if(model.getSplitCheck()) {
        MainWindow::splitAdd();
    }
    else {
        ui->playerHand->addPlayerCard(fileName);
        ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getUserTotal()));
    }
}

void MainWindow::splitHand() {
    ui->playerHand->splitPlayerCards();
    ui->splitScore->setVisible(true);
    ui->splitScore->setText("SPLIT SCORE: " + QString::number(model.split()));
    ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getSplitTotal()));
}

void MainWindow::nextSplit() {
    ui->playerHand->nextSplitHand();
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

void MainWindow::beginGame() {
    //Reset scores
    model.clearTotal();
    //Clear current cards
    ui->dealerHand->clearAllCards();
    ui->playerHand->clearAllCards();

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
    addPlayer();
    addPlayer();
}

 void MainWindow::onQuitGameClicked()
 {
     qDebug() << "clicked on quit game";
     this->close();
 }

 void MainWindow::switchToMainMenu() {
     ui->stackedWidget->setCurrentWidget(ui->startMenu);
 }

 void MainWindow::updateScores() {
     model.getDealerTotal();
     model.getUserTotal();
 }

 void MainWindow::switchToGameWindow() {
     ui->stackedWidget->setCurrentWidget(ui->game);
 }

 void MainWindow::stand() {
     model.getUserTotal();
     model.setSplitCheck(true);
 }

 void MainWindow::splitAdd() {
     QString fileName = QString::fromStdString(convertCardToPath(model.splitHit()));

     ui->playerHand->addPlayerCard(fileName);
     ui->splitScore->setText("SPLIT SCORE: " + QString::number(model.getSplitTotal()));
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
