#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , betTotal(0)
    , bankTotal(1500)
{
    ui->setupUi(this);

    connect(ui->addDealerButton, &QPushButton::clicked, this, &MainWindow::addDealer);
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &MainWindow::addPlayer);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(ui->add50, &QPushButton::clicked, this, &MainWindow::add50);
    connect(ui->add100, &QPushButton::clicked, this, &MainWindow::add100);
    connect(ui->add250, &QPushButton::clicked, this, &MainWindow::add250);
    connect(ui->add500, &QPushButton::clicked, this, &MainWindow::add500);

    ui->add50->setVisible(false);
    ui->add100->setVisible(false);
    ui->add250->setVisible(false);
    ui->add500->setVisible(false);
    ui->dealButton->setVisible(false);
    buttonState = true;

    ui->bank->setText("BANK: $" + QString::number(bankTotal));

    // EXAMPLE OF A HINT BOX
    helpwidget = new HelpWidget(this);
    helpwidget->setText("This is a very helpful tip on how to win the game!! blah blah blah blah blah blah. "
                        "Is this a sentence? Filler words. Filler words. More filler words.");
    helpwidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    if (helpwidget) {
        helpwidget->setSize();
    }
}

// Just example code for how you would call to add a card or clear them from the Box2D scene
void MainWindow::addDealer() {
    ui->dealerHand->addDealerCard(":/cards/10H.png");
}

void MainWindow::addPlayer() {
    ui->playerHand->addPlayerCard(":/cards/10C.png");
}

void MainWindow::clearAll() {
    ui->playerHand->clearAllCards();
    ui->dealerHand->clearAllCards();
}

void MainWindow::on_swapButtons_clicked()
{
    if (buttonState) {
        ui->add50->setVisible(true);
        ui->add100->setVisible(true);
        ui->add250->setVisible(true);
        ui->add500->setVisible(true);
        ui->dealButton->setVisible(true);

        ui->addPlayerButton->setVisible(false);
        ui->addDealerButton->setVisible(false);
        ui->clearAllButton->setVisible(false);
    }
    else {
        ui->add50->setVisible(false);
        ui->add100->setVisible(false);
        ui->add250->setVisible(false);
        ui->add500->setVisible(false);
        ui->dealButton->setVisible(false);

        ui->addPlayerButton->setVisible(true);
        ui->addDealerButton->setVisible(true);
        ui->clearAllButton->setVisible(true);
    }

    buttonState = !buttonState;
}

void MainWindow::add50() {
    if ((bankTotal - 50) > -1) {
        betTotal += 50;
        ui->currentBet->setText("CURRENT BET: $" + QString::number(betTotal));
        bankTotal -= 50;
        ui->bank->setText("BANK: $" + QString::number(bankTotal));
    }
}

void MainWindow::add100() {
    if ((bankTotal - 100) > -1) {
        betTotal += 100;
        ui->currentBet->setText("CURRENT BET: $" + QString::number(betTotal));
        bankTotal -= 100;
        ui->bank->setText("BANK: $" + QString::number(bankTotal));
    }
}

void MainWindow::add250() {
    if ((bankTotal - 250) > -1) {
        betTotal += 250;
        ui->currentBet->setText("CURRENT BET: $" + QString::number(betTotal));
        bankTotal -= 250;
        ui->bank->setText("BANK: $" + QString::number(bankTotal));
    }
}

void MainWindow::add500() {
    if ((bankTotal - 500) > -1) {
        betTotal += 500;
        ui->currentBet->setText("CURRENT BET: $" + QString::number(betTotal));
        bankTotal -= 500;
        ui->bank->setText("BANK: $" + QString::number(bankTotal));
    }
}

