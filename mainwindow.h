#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwidget.h"
#include "model.h"

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
    void addDealer();
    void addPlayer();
    void clearAll();
    void splitHand();
    void nextSplit();
    void onQuitGameClicked();
    void switchToGameWindow();
    void switchToMainMenu();
    void beginGame();
    void stand();
    void splitAdd();

private:
    Ui::MainWindow *ui;
    Model model;
    bool buttonState;
    int betTotal;
    int bankTotal;
    HelpWidget *helpwidget;

    // Probably will remove, serves as demo functions
    void setupConnections();
    void initializeUI();
    void createHelpWidget(QString);
    void toggleBetButtons(bool);
    void updateBankDisplay();
    void updateScores();
};
#endif // MAINWINDOW_H
