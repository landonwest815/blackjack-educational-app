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

    // Bet buttons
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });

    // Reset Bet buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);

    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);

    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);

//    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::beginGame);

    connect(ui->quitGameMenu , &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);

    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::hit);

    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::deal);

}

void MainWindow::initializeUI() {

    ui->stackedWidget->setCurrentWidget(ui->startMenu);

    toggleBetButtons(true);
    ui->dealButton->setVisible(true);
    ui->resetButton->setVisible(true);
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
void MainWindow::addDealer(bool facedown) {
    Card dealerCard = model.dealerHit(facedown);

    if (facedown) {
        dealerCard.setFaceDown(true);
    }

    model.addDealerCard(dealerCard);
    ui->dealerHand->addDealerCard(QString::fromStdString(convertCardToPath(dealerCard)));
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

}

void MainWindow::deal() {
    //Reset scores
    model.clearTotal();
    //Clear current cards
    ui->dealerHand->clearAllCards();
    ui->playerHand->clearAllCards();

    // Deal to player
    addPlayer();
    addPlayer();
    ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getUserTotal()));

    // Deal to dealer
    addDealer(true);
    addDealer(false);

    // Facedown card should not be included in score
    ui->dealerScore->setText("DEALER SCORE: " + QString::number(model.getDealerTotal()));

    /* Update UI */
    // Must make certain buttons and functions unavailable once the game starts and cards are dealt

    // Check for blackjack conditions
    if (model.getUserTotal() == 21) {
        stand();
    }
}

void MainWindow::hit() {
    addPlayer();

    if (model.getUserTotal() > 21) {
        model.dealerWins();
    } else if (model.getUserTotal() == 21) {
        stand();
    }
}

void MainWindow::stand() {
    // must flip over facedown card
    model.revealDealer();

    while (model.getDealerTotal() < 17 || (model.getDealerTotal() == 17 && model.getDealerAces() > 0)) {
        addDealer(false);
    }

    determineWinner();
    model.setSplitCheck(true);
}

void MainWindow::determineWinner() {
    int userTotal = model.getUserTotal();
    int dealerTotal = model.getDealerTotal();

    // Check for bust conditions
    if (userTotal > 21) {
        model.playerBust();
    } else if (dealerTotal > 21) {
        model.dealerBust();
    } else {
        // Compare hand values to determine the winner
        if (userTotal > dealerTotal) {
            model.playerWins();
        } else if (userTotal < dealerTotal) {
            model.dealerWins();
        } else {
            model.handlePush(); // when a tie occurs
        }
    }

    updateBankDisplay();
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

 void MainWindow::splitAdd() {
     QString fileName = QString::fromStdString(convertCardToPath(model.splitHit()));

     ui->playerHand->addPlayerCard(fileName);
     ui->splitScore->setText("SPLIT SCORE: " + QString::number(model.getSplitTotal()));
 }
