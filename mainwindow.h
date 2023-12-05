#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwidget.h"
#include "model.h"
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QtTextToSpeech/QTextToSpeech>

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
    void displayAdvice();
    void tellUserToHit();
    void tellUserToStand();
    void tellUserToDoubleDownOrHit();
    void tellUserToDoubleDownOrStand();
    void tellUserToSplit();
    void tellUserToSplitOrHit();
    void hideTip();

    // just for demonstration REMOVE when needed
    void nextSplit();
    void dealerFlip(QString fileName);

    void lostGame();

    void settingsClicked();

    void speechModeSettingAdjusted(const QString &arg1);

    void themeChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    Model model;
    bool buttonState;
    int betTotal;
    int bankTotal;
    HelpWidget *helpwidget;
    QList<QPushButton*> buttons;
    QList<QLabel*> labels;
    QTimer tipTimer;

    void determineWinner();
    // Probably will remove, serves as demo functions
    void setupConnections();
    void initializeUI();
    void createHelpWidget(QString);
    void updateBankDisplay();
    void updateScores();
    void showATip();
    int dealerFaceUpValue;
    string dealerFaceUpSuit;
    void hideAllUI();
    void setupDeal();
    void showOutcome(QString outcome, bool splitHand);
    void insurance();
    void newGame();
    bool checkForDuplicateUserHand();

    /// Keeps track of current tutorial step.
    int tutorialStep;

    /// Flag indicating whether speech mode is enabled.
    bool speech;

    /// Text-to-speech object.
    QTextToSpeech say;

    /// @brief Enables or disables speech mode.
    /// @param mode The speech mode (1 for enable, 0 for disable).
    void speechModeClicked(bool mode);

    /// @brief When the speech assistant is activated, the assistant says the object's name.
    void sayObjectName();

};
#endif // MAINWINDOW_H
