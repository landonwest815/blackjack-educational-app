/***
 * Authors:         Team Six of Hearts
 * Members:         Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *                  Landon West, and Pablo Arancibia-Bazan
 * Course:          CS 3505, University of Utah, School of Computing
 * Assignment:      A9 - Educational App
 *
 * Description:     Header for the main window of a Blackjack game application.
 *                  It handles user interactions, manages the game state, and updates the UI
 *                  accordingly. The class covers functionalities like dealing cards, placing bets,
 *                  making game decisions (hit, stand, split, double down), and navigating through
 *                  different screens of the application.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwidget.h"
#include "model.h"
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QtTextToSpeech/QTextToSpeech>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///@brief The MainWindow class manages the main interface of the application, including
///       gameplay, user interaction, and navigation between different screens.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    ///@brief Constructs the main window.
    MainWindow(QWidget *parent = nullptr);
    ///@brief Destructs the main window.
    ~MainWindow();

private slots:

    // BETTING
    ///@brief Increments the current bet by a specified amount.
    void addToBet(int increment);
    ///@brief Resets the current bet to the minimum. Adds remaining amount to the bank.
    void resetBet();

    // TURN BASED ACTIONS
    ///@brief Adds a card to the dealer's hand. Can be facedown.
    void addDealer(bool facedown);
    ///@brief Adds a card to the player's hand.
    void addPlayer();
    ///@brief Clears all cards from the table.
    void clearAll();
    ///@brief Splits the player's hand in case of two cards of the same value.
    void splitHand();
    ///@brief Indicates that the player ends their turn.
    void stand();
    ///@brief Doubles the bet and gives the player one final card.
    void doubleDownHand();
    ///@brief Deals the initial cards at the beginning of a game.
    void deal();
    ///@brief Handles insurance bets in the game.
    void insurance();
    ///@brief Starts a new game with a fresh setup.
    void newGame();
    ///@brief Displays the statistics sheet.
    void showSheet();
    ///@brief Displays current turn advice to the player.
    void displayAdvice();

    // MENUS
    ///@brief Handles the event when the 'Quit Game' option is clicked.
    void onQuitGameClicked();
    ///@brief Switches the view to the game window.
    void switchToGameWindow();
    ///@brief Switches the view to the lessons window.
    void switchToLessonsWindow();
    ///@brief Switches the view to the main menu.
    void switchToMainMenu();
    ///@brief Handles the event when the 'Settings' option is clicked.
    void settingsClicked();

    // LESSON TRAVERSAL
    ///@brief Handles the selection of a lesson.
    void handleLessonSelect();
    ///@brief Advances to the next step in lesson one.
    void nextLessonOneStep();
    ///@brief Advances to the next step in lesson two.
    void nextLessonTwoStep();
    ///@brief Advances to the next step in lesson three.
    void nextLessonThreeStep();

    // SETTINGS
    ///@brief Turns the speech mode setting on or off.
    void speechModeSettingAdjusted(const QString &arg1);
    ///@brief Changes the theme of the application.
    void themeChanged(const QString &arg1);

private:
    // VARIABLE MEMBERS
    ///@brief Holds the UI elements of the MainWindow.
    Ui::MainWindow *ui;
    ///@brief The model handling the game logic and data.
    Model model;
    ///@brief Help widget for displaying tips and advice.
    HelpWidget *helpwidget;
    ///@brief Holds all push buttons in the UI.
    QList<QPushButton*> buttons;
    ///@brief Holds all dynamic labels in the UI.
    QList<QLabel*> labels;
    ///@brief Timer for showing and hiding tips.
    QTimer tipTimer;
    ///@brief Location of the sheet (rule/info sheet).
    QString sheetLocation;
    ///@brief Current step in the tutorial process.
    int tutorialStep;
    ///@brief Indicates whether speech mode is enabled.
    bool speech;
    ///@brief Text-to-speech object for voice feedback.
    QTextToSpeech say;

    // INITIAL SETUP
    ///@brief Sets up signal-slot connections for the UI elements.
    void setupConnections();
    ///@brief Initializes the UI components.
    void initializeUI();
    ///@brief Creates a help widget with specified text.
    void createHelpWidget(QString);
    ///@brief Initializes a group of related buttons.
    void initializeButtonGroup();
    ///@brief Initializes a group of related labels.
    void initializeLabelGroup();

    // GAME STATES
    ///@brief Determines the winner of the current hand.
    void determineWinner();
    ///@brief Prepares the UI for a new deal.
    void setupDeal();
    ///@brief Checks for duplicate cards in the user's hand.
    bool checkForDuplicateUserHand();
    ///@brief Handles the addition of a card in a split hand scenario.
    void splitAdd();
    ///@brief Handles the transition to the next split hand.
    void nextSplit();
    ///@brief Flips the dealer's card, revealing it.
    void dealerFlip(QString fileName);
    ///@brief Handles the scenario when the player loses the game.
    void lostGame();

    // VISUAL UPDATES
    ///@brief Updates the bank balance and bet amount display in the UI.
    void updateBankDisplay();
    ///@brief Updates the score displays in the UI.
    void updateScores();
    ///@brief Shows advice to the user.
    void showATip();
    ///@brief Hides previously shown advice to the user.
    void hideTip();
    ///@brief Hides all UI elements in buttons and labels groups.
    void hideAllUI();
    ///@brief Displays the outcome of the hand.
    void showOutcome(QString outcome, bool splitHand, bool win);

    // SPEECH
    /// @brief Enables or disables speech mode.
    /// @param mode The speech mode (1 for enable, 0 for disable).
    void speechModeClicked(bool mode);
    /// @brief When the speech assistant is activated, the assistant says the object's name.
    void sayObjectName();

    // OTHER HELPERS
    ///@brief Handles the window resize event.
    void resizeEvent(QResizeEvent *event) override;
    ///@brief Converts a card object to its image path.
    QString convertCardToPath(Card card);

    // ADVICE
    ///@brief Advises the user to hit (take another card).
    void tellUserToHit();
    ///@brief Advises the user to stand (not take any more cards).
    void tellUserToStand();
    ///@brief Advises the user to either double down or hit.
    void tellUserToDoubleDownOrHit();
    ///@brief Advises the user to either double down or stand.
    void tellUserToDoubleDownOrStand();
    ///@brief Advises the user to split their hand.
    void tellUserToSplit();
    ///@brief Advises the user to either split or hit.
    void tellUserToSplitOrHit();
};
#endif // MAINWINDOW_H
