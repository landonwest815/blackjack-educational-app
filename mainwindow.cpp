#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage image(":/cards/5H.png");
    QPixmap pix;
    pix.convertFromImage(image);

    ui->dealerCards->setPixmap(pix);


    QImage image2(":/cards/QS.png");
    QPixmap pix2;
    pix2.convertFromImage(image2);

    ui->playerCards->setPixmap(pix2);

}

MainWindow::~MainWindow()
{
    delete ui;


}

