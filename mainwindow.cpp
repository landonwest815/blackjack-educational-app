#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addDealerButton, &QPushButton::clicked, this, &MainWindow::addDealer);
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &MainWindow::addPlayer);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &MainWindow::clearAll);
}

MainWindow::~MainWindow()
{
    delete ui;


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
