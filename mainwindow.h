#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwidget.h"
#include "model.h"
#include <QPushButton>
#include <QLabel>

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
    void switchToLessonsWindow();

    void handleLessonSelect();

    void nextLessonOneStep();
    void nextLessonTwoStep();
    void nextLessonThreeStep();


    void switchToMainMenu();
    void splitAdd();
    void deal();
    void stand();
    void doubleDownHand();
    void doubleDown();
    void displayAdvice();

    // just for demonstration REMOVE when needed
    void nextSplit();
    void dealerFlip(QString fileName);

private:
    Ui::MainWindow *ui;
    Model model;
    bool buttonState;
    int betTotal;
    int bankTotal;
    HelpWidget *helpwidget;
    QList<QPushButton*> buttons;
    QList<QLabel*> labels;

    void determineWinner();
    // Probably will remove, serves as demo functions
    void setupConnections();
    void initializeUI();
    void createHelpWidget(QString);
    void updateBankDisplay();
    void updateScores();
    void hideAllUI();
    void setupDeal();
    void showOutcome(QString outcome);
    void insurance();

    /// Keeps track of current tutorial step.
    int tutorialStep;

};
#endif // MAINWINDOW_H
