#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buttons = {ui->hitButton, ui->standButton, ui->splitButton, ui->nextSplitButton, ui->flipDealerButton, ui->doubleDemoButton,
               ui->add50, ui->add100, ui->add250, ui->add500, ui->resetButton, ui->dealButton, ui->insuranceButton, ui->nextHand,
               ui->allIn};

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
    //connect(ui->flipDealerButton, &QPushButton::clicked, this, &MainWindow::dealerFlip);
    connect(ui->doubleDemoButton, &QPushButton::clicked, this, &MainWindow::doubleDownHand);

    // Bet buttons
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });
    connect(ui->allIn, &QPushButton::clicked, this, [this](){ addToBet(model.getbankTotal()); });

    // Reset Bet buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);

    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);

    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);

//    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::beginGame);

    connect(ui->quitGameMenu , &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);

    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::hit);

    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::deal);
    connect(ui->nextHand, &QPushButton::clicked, this, &MainWindow::setupDeal);

}

void MainWindow::initializeUI() {

    ui->stackedWidget->setCurrentWidget(ui->startMenu);
    ui->splitScore->setVisible(false);
    ui->outcome->setVisible(false);

    hideAllButtons();

    updateBankDisplay();
}

void MainWindow::hideAllButtons() {
    for (QPushButton* button : buttons) {
        button->setVisible(false);
    }
    ui->outcome->setVisible(false);
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

    //qDebug() << fileName;

    if(model.getSplitCheck()) {
        MainWindow::splitAdd();
    }
    else {
        ui->playerHand->addPlayerCard(fileName);
        ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getUserTotal()));
        if (model.getUserTotal() > 21) {
            determineWinner();
        }
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

    hideAllButtons();

    ui->hitButton->setVisible(true);
    ui->standButton->setVisible(true);

    // Check for blackjack conditions
    if (model.getUserTotal() == 21) {
        stand();
    }
}

void MainWindow::hit() {
    addPlayer();

    if (model.getUserTotal() > 21) {
        stand();
    } else if (model.getUserTotal() == 21) {
        stand();
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
    model.setSplitCheck(true);
}

void MainWindow::determineWinner() {
    int userTotal = model.getUserTotal();
    int dealerTotal = model.getDealerTotal();

    // Check for bust conditions
    if (userTotal > 21) {
        model.playerBust();
        hideAllButtons();
        ui->outcome->setVisible(true);
        ui->outcome->setText("PLAYER BUSTS!");
        ui->nextHand->setVisible(true);
    } else if (dealerTotal > 21) {
        model.dealerBust();
        hideAllButtons();
        ui->outcome->setVisible(true);
        ui->outcome->setText("DEALER BUSTS!");
        ui->nextHand->setVisible(true);
    } else {
        // Compare hand values to determine the winner
        if (userTotal > dealerTotal) {
            model.playerWins();
            hideAllButtons();
            ui->outcome->setVisible(true);
            ui->outcome->setText("PLAYER WINS!");
            ui->nextHand->setVisible(true);
        } else if (userTotal < dealerTotal) {
            model.dealerWins();
            hideAllButtons();
            ui->outcome->setVisible(true);
            ui->outcome->setText("DEALER WINS!");
            ui->nextHand->setVisible(true);
        } else {
            model.handlePush(); // when a tie occurs
            model.dealerWins();
            hideAllButtons();
            ui->outcome->setVisible(true);
            ui->outcome->setText("PUSH!");
            ui->nextHand->setVisible(true);
        }
    }

    updateBankDisplay();
}

 void MainWindow::onQuitGameClicked()
 {
     //qDebug() << "clicked on quit game";
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
     setupDeal();
 }

 void MainWindow::setupDeal() {
     // prompt the bet amounts
     hideAllButtons();
     clearAll();

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
     ui->splitScore->setText("SPLIT SCORE: " + QString::number(model.getSplitTotal()));
 }
