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
    createHelpWidget("This is a very helpful tip on how to win the game!! blah blah blah blah blah blah. "
                  "Is this a sentence? Filler words. Filler words. More filler words.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {

    // Card buttons
    connect(ui->addDealerButton, &QPushButton::clicked, this, &MainWindow::addDealer);
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &MainWindow::addPlayer);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::splitHand);

    // Bet buttons
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });

    // Reset Bet buttons
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);

    // Swap button
    connect(ui->swapButtons, &QPushButton::clicked, this, &MainWindow::swapButtons);

    // Quit Game buttong
    connect(ui->quitGame , &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);
}

void MainWindow::initializeUI() {

    toggleBetButtons(false);
    ui->dealButton->setVisible(false);
    ui->resetButton->setVisible(false);
    buttonState = true;

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
    if (helpwidget) helpwidget->setSize();
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

    ui->playerHand->addPlayerCard(fileName);
    ui->playerScore->setText("PLAYER SCORE: " + QString::number(model.getUserTotal()));
}

void MainWindow::splitHand() {
    ui->playerHand->splitPlayerCards();
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
    model.clearTotal();
}

void MainWindow::swapButtons()
{
    // Bet Buttons
    QPushButton* betButtons[6] = {
        ui->add50, ui->add100, ui->add250, ui->add500,
        ui->dealButton, ui->resetButton,
    };

    for (QPushButton* button : betButtons) {
        button->setVisible(buttonState);
    }

    // Card Buttons
    QPushButton* cardButtons[3] = {
        ui->addPlayerButton, ui->addDealerButton, ui->clearAllButton
    };

    for (QPushButton* button : cardButtons) {
        button->setVisible(!buttonState);
    }

    buttonState = !buttonState;
}

void MainWindow::addToBet(int increment) {
    model.setBet(increment);
    updateBankDisplay();
}

void MainWindow::resetBet() {
    model.resetBet();
    updateBankDisplay();
}

 void MainWindow::onQuitGameClicked()
 {
     qDebug() << "clicked on quit game";
     this->close();
 }

 void MainWindow::updateScores() {
     model.getDealerTotal();
     model.getUserTotal();
 }
