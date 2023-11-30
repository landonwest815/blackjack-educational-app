#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwidget.h"
#include "model.h"
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event) override;
    string convertCardToPath(Card card);

private slots:
    void addToBet(int increment);
    void resetBet();
    void addDealer(bool facedown);
    void addPlayer();
    void clearAll();
    void splitHand();
    void onQuitGameClicked();
    void switchToGameWindow();
    void switchToMainMenu();
    void beginGame();
    void splitAdd();
    void deal();
    void hit();
    void stand();

    // just for demonstration REMOVE when needed
    void nextSplit();
    void dealerFlip(QString fileName);
    void doubleDownHand();


private:
    Ui::MainWindow *ui;
    Model model;
    bool buttonState;
    int betTotal;
    int bankTotal;
    HelpWidget *helpwidget;
    QList<QPushButton*> buttons;

    void determineWinner();
    // Probably will remove, serves as demo functions
    void setupConnections();
    void initializeUI();
    void createHelpWidget(QString);
    void updateBankDisplay();
    void updateScores();
    void hideAllButtons();
    void setupDeal();
};
#endif // MAINWINDOW_H
