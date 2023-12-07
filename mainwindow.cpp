#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwidget.h"
#include "QDebug"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tutorialStep(1)
    , speech(false)
    , sheetLocation(":/cards/sheetCasino.png")
{
    ui->setupUi(this);

    // Contains all buttons so we can turn all of them off at once
    buttons = {ui->hitButton, ui->standButton, ui->splitButton, ui->nextSplitButton, ui->doubleDownButton,
               ui->add50, ui->add100, ui->add250, ui->add500, ui->resetButton, ui->dealButton, ui->insuranceButton, ui->nextHand,
               ui->allIn, ui->nextLessonOne, ui->nextLessonTwo, ui->nextLessonThree, ui->adviceButton, ui->newGame, ui->sheetButton};

    // Contains all labels so we can turn all of them off at once
    labels = {ui->outcome, ui->splitScore, ui->tutorialLabel, ui->splitIndicator, ui->nonSplitIndicator, ui->insuranceResult,
              ui->splitOutcome, ui->tutorialTextLabel};

    setupConnections();
    initializeUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {

    // Turn Actions
    connect(ui->hitButton, &QPushButton::clicked, this, &MainWindow::addPlayer);
    connect(ui->standButton, &QPushButton::clicked, this, &MainWindow::stand);

    // Splitting
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::splitHand);
    connect(ui->nextSplitButton, &QPushButton::clicked, this, &MainWindow::nextSplit);

    // End of Turn
    connect(ui->nextHand, &QPushButton::clicked, this, &MainWindow::setupDeal);

    // Betting
    connect(ui->add50, &QPushButton::clicked, this, [this](){ addToBet(50); });
    connect(ui->add100, &QPushButton::clicked, this, [this](){ addToBet(100); });
    connect(ui->add250, &QPushButton::clicked, this, [this](){ addToBet(250); });
    connect(ui->add500, &QPushButton::clicked, this, [this](){ addToBet(500); });
    connect(ui->allIn, &QPushButton::clicked, this, [this](){ addToBet(model.getbankTotal()); });
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetBet);
    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::deal);
    connect(ui->insuranceButton, &QPushButton::clicked, this, &MainWindow::insurance);
    connect(ui->doubleDownButton, &QPushButton::clicked, this, &MainWindow::doubleDownHand);

    // Menus
    connect(ui->mainMenu , &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->mainMenuButton, &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->mainMenuSettingButton, &QPushButton::clicked, this, &MainWindow::switchToMainMenu);
    connect(ui->startGame, &QPushButton::clicked, this, &MainWindow::switchToGameWindow);
    connect(ui->tutorial, &QPushButton::clicked, this, &MainWindow::switchToLessonsWindow);
    connect(ui->quitGameMenu, &QPushButton::clicked, this, &MainWindow::onQuitGameClicked);
    connect(ui->adviceButton, &QPushButton::clicked, this, &MainWindow::displayAdvice);
    connect(ui->sheetButton, &QPushButton::clicked, this, &MainWindow::showSheet);
    connect(ui->newGame, &QPushButton::clicked, this, &MainWindow::newGame);
    connect(ui->settings, &QPushButton::clicked, this ,&MainWindow::settingsClicked);

    // Lessons
    connect(ui->lessonOne, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);
    connect(ui->lessonTwo, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);
    connect(ui->lessonThree, &QPushButton::clicked, this, &MainWindow::handleLessonSelect);

    // Lesson Traversal
    connect(ui->nextLessonOne, &QPushButton::clicked, this, &MainWindow::nextLessonOneStep);
    connect(ui->nextLessonTwo, &QPushButton::clicked, this, &MainWindow::nextLessonTwoStep);
    connect(ui->nextLessonThree, &QPushButton::clicked, this, &MainWindow::nextLessonThreeStep);

    // Set up tip timer
    connect(&tipTimer, &QTimer::timeout, this, &MainWindow::hideTip);

    // For the speech mode
    connect(ui->speechComboBox, &QComboBox::currentTextChanged, this, &MainWindow::speechModeSettingAdjusted);

    // For the theme
    connect(ui->themeComboBox, &QComboBox::currentTextChanged, this, &MainWindow::themeChanged);

    // Connect all button clicks to a speech slot
    QList<QPushButton*> allButtons = findChildren<QPushButton*>();
    for (QPushButton* button : allButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::sayObjectName);
    }
}

void MainWindow::initializeUI() {

    // Set the screen to the Main Menu
    ui->stackedWidget->setCurrentWidget(ui->startMenu);

    // Initially hide all game buttons and labels
    hideAllUI();

    // Set the player and dealer hand attributes
    ui->playerHand->setIsPlayerHand(true);
    ui->dealerHand->setIsPlayerHand(false);

    // Set the bank
    updateBankDisplay();

    // set up tips
    createHelpWidget("");
    tipTimer.setSingleShot(true);
}

void MainWindow::hideAllUI() {
    for (QPushButton* button : buttons) {
        button->setVisible(false);
    }
    for (QLabel* label : labels) {
        label->setVisible(false);
    }
}

void MainWindow::updateBankDisplay() {
    ui->bank->setText("BANK: $" + QString::number(model.getbankTotal()));
    ui->currentBet->setText("BET: $" + QString::number(model.getBet()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Resize the advice
    if (helpwidget) helpwidget->setSize();
}

void MainWindow::createHelpWidget(QString text) {
    helpwidget = new HelpWidget(this);
    helpwidget->setText(text);

    // Hide it until we want to show it
    helpwidget->hide();
}

void MainWindow::addDealer(bool facedown) {

    // Draw a random card
    Card dealerCard = model.dealerHit(facedown);
    model.addDealerCard(dealerCard);

    // Add it to the hand
    ui->dealerHand->addDealerCard(QString::fromStdString(convertCardToPath(dealerCard)));
    updateScores();
}

void MainWindow::addPlayer() {

    // Check if we are on a split hand
    if (!model.getOnSecondHand()) {
        // Draw a random card and add it to the hand
        Card playerCard = model.userHit();
        model.addUserCard(playerCard);
        ui->playerHand->addPlayerCard(QString::fromStdString(convertCardToPath(playerCard)));
    }
    else {
        // Draw a random card and add it to the split hand
        Card playerCard = model.splitHit();
        model.addUserSplitCard(playerCard);
        ui->playerHand->addPlayerCard(QString::fromStdString(convertCardToPath(playerCard)));
    }

    // Get rid of option to opt for insurance or doubling down
    ui->insuranceButton->setVisible(false);
    ui->doubleDownButton->setVisible(false);
    updateScores();

    // End round if player hits 21
    if (model.getUserTotal() >= 21) {
        if (!model.getSplitCheck()) {
            stand();
        } else if (!model.getOnSecondHand() && model.getSplitCheck()) {
            nextSplit();
        }
    }

    if (model.getSplitTotal() >= 21) {
        stand();
    }

    ui->insuranceResult->setVisible(false);
    helpwidget->hide();
}

void MainWindow::splitHand() {

    // Tell Box2D we have split our hand
    ui->playerHand->splitPlayerCards();

    // Update UI for split score
    ui->splitScore->setVisible(true);
    ui->nextSplitButton->setVisible(true);
    ui->nonSplitIndicator->setVisible(true);
    ui->splitButton->setVisible(false);

    helpwidget->hide();
    model.split();
    updateScores();
}

void MainWindow::nextSplit() {

    // Tell Box2D we have moved to the next hand
    ui->playerHand->nextSplitHand();

    // Update UI to show that
    ui->nonSplitIndicator->setVisible(false);
    ui->nextSplitButton->setVisible(false);
    ui->splitIndicator->setVisible(true);

    helpwidget->hide();

    // Update model
    model.setOnSecondHand(true);
}

void MainWindow::dealerFlip(QString fileName) {
    ui->dealerHand->flipDealerCard(fileName);
}

void MainWindow::doubleDownHand() {

    Card doubleDownCard = model.doubleDown();
    ui->playerHand->doubleDownPlayerCard(QString::fromStdString(convertCardToPath(doubleDownCard)));

    updateScores();
    stand();
}

string MainWindow::convertCardToPath(Card card) {

    // Pull out the attributes
    string suit = card.getSuit();
    int value = card.getValue();
    string face = card.getFace();

    // Create the path
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

    // Clear both hands
    ui->playerHand->clearAllCards();
    ui->dealerHand->clearAllCards();

    // Reset the score
    ui->playerScore->setText(QString::number(0));
    ui->dealerScore->setText(QString::number(0));
    ui->splitScore->setText(QString::number(0));
    model.resetAllScores();

    // Reset the split data
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

void MainWindow::deal() {

    model.setBlackJack(true);

    // Give the dealer two cards (one of which is face down)
    addDealer(true);
    qDebug() << QString::number(model.getDealerTotal()) + " 3";
    addDealer(false);
    qDebug() << QString::number(model.getDealerTotal()) + " 4";

    // Give the player two cards
    addPlayer();
    qDebug() << QString::number(model.getDealerTotal()) + " 1";
    addPlayer();
    qDebug() << QString::number(model.getDealerTotal()) + " 2";

    // Set neither to be able to shake as of now
    // This will be triggered after a bust
    ui->playerHand->setShakingEnabled(false);
    ui->dealerHand->setShakingEnabled(false);

    model.setBlackJack(false);

    if (model.getUserTotal() != 21) {

        updateScores();

        // Update the UI
        hideAllUI();

        ui->hitButton->setVisible(true);
        ui->standButton->setVisible(true);
        ui->doubleDownButton->setVisible(true);
        ui->adviceButton->setVisible(true);
        ui->sheetButton->setVisible(true);
        if (model.allowedToSplit()) ui->splitButton->setVisible(true);
        if(model.insuranceAllowed()) ui->insuranceButton->setVisible(true);
    }
}

void MainWindow::stand() {
    // must flip over facedown card
    dealerFlip(QString::fromStdString(convertCardToPath(model.revealDealer())));
    qDebug() << QString::number(model.getDealerTotal()) + " 5";
    updateScores();

    // dealer draws until it has a score of 17 or higher
    while (model.getDealerTotal() < 17 || (model.getDealerTotal() == 17 && model.getDealerAces() > 0)) {
        addDealer(false);
        qDebug() << QString::number(model.getDealerTotal()) + " 6";
    }

    helpwidget->hide();
    determineWinner();
}

void MainWindow::determineWinner() {
    int userTotal;
    bool splitHand = model.getOnSecondHand();

    if (splitHand) {
        userTotal = model.getSplitTotal();
    }
    else userTotal = model.getUserTotal();

    int dealerTotal = model.getDealerTotal();

    // Check for bust conditions
    if (model.isBlackJack()){
        model.playerWins();
        // Show chips falling
        ui->playerHand->setPlayerWon(true);
        showOutcome("BLACKJACK!", splitHand);

    } else if (userTotal > 21) {
        model.dealerWins();
        showOutcome("PLAYER BUSTS!", splitHand);
        // Shake the cards
        QTimer::singleShot(700, this, [this]() {
            ui->playerHand->setShakingEnabled(true);
        });

    } else if (dealerTotal > 21) {
        model.playerWins();
        // Show chips falling
        ui->playerHand->setPlayerWon(true);
        showOutcome("DEALER BUSTS!", splitHand);
        // Shake the cards
        QTimer::singleShot(700, this, [this]() {
            ui->dealerHand->setShakingEnabled(true);
        });

    } else {
        if (userTotal > dealerTotal) {
            model.playerWins();
            showOutcome("PLAYER WINS!", splitHand);
            // Show chips falling
            ui->playerHand->setPlayerWon(true);

        } else if (userTotal < dealerTotal) {
            model.dealerWins();
            showOutcome("DEALER WINS!", splitHand);

        } else {
            model.handlePush(); // when a tie occurs
            //model.dealerWins();
            showOutcome("PUSH!", splitHand);
        }
    }

    if (model.getOnSecondHand()) {
        model.setOnSecondHand(false);
        determineWinner();
    }

    ui->sheetButton->setVisible(false);
    ui->adviceButton->setVisible(false);
    updateBankDisplay();
}

void MainWindow::showOutcome(QString outcome, bool splitHand) {
    //hideAllUI();
    ui->hitButton->setVisible(false);
    ui->standButton->setVisible(false);
    ui->add50->setVisible(false);
    ui->add100->setVisible(false);
    ui->add250->setVisible(false);
    ui->add500->setVisible(false);
    ui->allIn->setVisible(false);
    ui->resetButton->setVisible(false);
    ui->dealButton->setVisible(false);
    ui->doubleDownButton->setVisible(false);

    if (!splitHand) {
        ui->outcome->setVisible(true);
        ui->outcome->setText(outcome);
        qDebug() << "normal outcome";
    } else {
        ui->splitOutcome->setVisible(true);
        ui->splitOutcome->setText(outcome);
        ui->splitIndicator->setVisible(false);
        qDebug() << "split outcome";
    }
    ui->nextHand->setVisible(true);
}

 void MainWindow::onQuitGameClicked()
 {
     this->close();
 }

 void MainWindow::switchToMainMenu() {
     ui->stackedWidget->setCurrentWidget(ui->startMenu);
     ui->playerHand->setIsInTutorial(false);
     ui->dealerHand->setIsInTutorial(false);
     tutorialStep = 1;

     ui->tutorialTextLabel->setVisible(false);
     ui->playerHand->setPlayerWon(false);
     ui->currentBet->setVisible(true);
     ui->bank->setVisible(true);
     ui->playerScore->setVisible(true);
     ui->dealerScore->setVisible(true);
     helpwidget->hide();

     // Reset the model and clear all cards
     model = Model();
     clearAll();
     ui->playerHand->clearDiscardPile();
     ui->dealerHand->clearDiscardPile();
 }

 void MainWindow::updateScores() {
     ui->dealerScore->setText(QString::number(model.getDealerTotal()));
     ui->playerScore->setText(QString::number(model.getUserTotal()));
     ui->splitScore->setText(QString::number(model.getSplitTotal()));
 }

 void MainWindow::switchToGameWindow() {
     ui->stackedWidget->setCurrentWidget(ui->game);
     setupDeal();
 }

 void MainWindow::switchToLessonsWindow() {
     ui->stackedWidget->setCurrentWidget(ui->lessons);
     ui->playerHand->setIsInTutorial(true);
     ui->dealerHand->setIsInTutorial(true);
 }

 void MainWindow::setupDeal() {
     // prompt the bet amounts
     hideAllUI();
     clearAll();
     model.endRound();
     addToBet(100);

     // Make sure there are no special effect still active
     ui->playerHand->setShakingEnabled(false);
     ui->dealerHand->setShakingEnabled(false);
     ui->nonSplitIndicator->setVisible(false);
     ui->splitIndicator->setVisible(false);
     ui->playerHand->setPlayerWon(false);

     // "Shuffle" the discard pile if necessary
     if (model.shuffleCheck()) {
        ui->playerHand->clearDiscardPile();
        ui->dealerHand->clearDiscardPile();
        tipTimer.start(4000);
        helpwidget->setText("SHUFFLE!");
        helpwidget->show();
     }

     // Check if user ran out of money
     if (model.getbankTotal() == 0 && model.getBet() == 0) {
        lostGame();
        return;
     }

     // Show Betting Actions
     ui->add50->setVisible(true);
     ui->add100->setVisible(true);
     ui->add250->setVisible(true);
     ui->add500->setVisible(true);
     ui->allIn->setVisible(true);
     ui->resetButton->setVisible(true);
     ui->dealButton->setVisible(true);

     updateScores();
 }

 void MainWindow::splitAdd() {
     QString fileName = QString::fromStdString(convertCardToPath(model.splitHit()));
     ui->playerHand->addPlayerCard(fileName);
     updateScores();
 }

 void MainWindow::showATip(){
     qDebug() << "ran showATip()";
     int usersTotal = model.getUserTotal();
     int dealerFaceUpValue = model.getDealerFaceUpCard().getValue();
     if(model.userHasAceInHand()){
        if(model.getTotalCardsForUser() > 2){
            if(usersTotal >= 17){
                tellUserToStand();
                return;
            }
        }
        // this just checks for a case where there are two aces so the user would be best to go for a split
        if(model.getTotalCardsForUser() == 2 && ((model.getUserCard(0).getFace() == "A") && (model.getUserCard(1).getFace() == "A"))){
            tellUserToSplit();
            return;
        }
        if((usersTotal == 3 || usersTotal == 13) ||
           (usersTotal == 4 || usersTotal == 14)){
            if(dealerFaceUpValue == 5 || dealerFaceUpValue == 6){
                tellUserToDoubleDownOrHit();
            } else{
                tellUserToHit();
            }
        }
        else if((usersTotal == 5 || usersTotal == 15) ||
                (usersTotal == 6 || usersTotal == 16)){
            if(dealerFaceUpValue == 5|| dealerFaceUpValue == 6 ||
                dealerFaceUpValue == 4){
                tellUserToDoubleDownOrHit();
            } else{
                tellUserToHit();
            }

        }
        else if(usersTotal == 7 || usersTotal == 17){
            if(dealerFaceUpValue == 3 || dealerFaceUpValue == 5 || dealerFaceUpValue == 6 ||
                dealerFaceUpValue == 4){
                tellUserToDoubleDownOrHit();
            } else{
                tellUserToHit();
            }
        }
        else if(usersTotal >= 8 || usersTotal >= 18){
            if((usersTotal == 8 || usersTotal == 18) &&
                (dealerFaceUpValue >= 3 && dealerFaceUpValue <= 6))
            {
                tellUserToDoubleDownOrStand();
            }
            else if((usersTotal == 8 || usersTotal == 18) &&
                       (dealerFaceUpValue >= 9 || model.getDealerFaceUpCard().getFace() == "A")){
                tellUserToHit();
            }
            else {
                tellUserToStand();
            }
        }
     }
     else{
        if(model.getTotalCardsForUser() == 2){
            if(checkForDuplicateUserHand()){
                return;
            }
        }
        if(usersTotal <= 8){
            tellUserToHit();
        }
        else if(usersTotal > 8 && usersTotal <= 12){
            if(usersTotal == 11){
                tellUserToDoubleDownOrHit();
            }
            else if((usersTotal == 10) &&
                       (dealerFaceUpValue >= 2 && dealerFaceUpValue <=9)){
                tellUserToDoubleDownOrHit();
            }
            else if((usersTotal == 9) &&
                       (dealerFaceUpValue >= 3 && dealerFaceUpValue <= 6)){
                tellUserToDoubleDownOrHit();
            }
            else if((usersTotal == 12) &&
                       (dealerFaceUpValue >= 4 && dealerFaceUpValue <= 6)){
                tellUserToStand();
            }
            else{
                tellUserToHit();
            }
        }
        else if(usersTotal >= 13 && usersTotal <= 16){
            if(dealerFaceUpValue >= 2 && dealerFaceUpValue <= 6){
                tellUserToStand();
            }
            else{
                tellUserToHit();
            }
        }
        else if(usersTotal >= 17){
            tellUserToStand();
        }
     }

 }

 bool MainWindow::checkForDuplicateUserHand(){
     Card firstCard = model.getUserCard(0);
     Card secondCard = model.getUserCard(1);
     if(firstCard.getValue() == 8 && secondCard.getValue() == 8){
        tellUserToSplit();
        return true;
     }
     else if((firstCard.getValue() == 2 && secondCard.getValue() == 2) ||
              (firstCard.getValue() == 3 && secondCard.getValue() == 3)){
        if(dealerFaceUpValue == 2 || dealerFaceUpValue == 3){
            tellUserToSplitOrHit();
        }
        else if(dealerFaceUpValue >= 4 && dealerFaceUpValue <= 7){
            tellUserToSplit();
        }
        else{
            tellUserToHit();
        }
        return true;
     }
     else if(firstCard.getValue() == 4 && secondCard.getValue() == 4){
        if((dealerFaceUpValue >= 2 && dealerFaceUpValue <= 4) ||
            (dealerFaceUpValue >= 7) || (model.getDealerFaceUpCard().getFace() == "A")){
            tellUserToHit();
        }
        else{
            tellUserToSplitOrHit();
        }
        return true;
     }
     else if(firstCard.getValue() == 5 && secondCard.getValue() == 5){
        if(dealerFaceUpValue <= 9){
            tellUserToDoubleDownOrHit();
        }
        else{
            tellUserToHit();
        }
        return true;
     }
     else if(firstCard.getValue() == 6 && secondCard.getValue() == 6){
        if(dealerFaceUpValue == 2){
            tellUserToSplitOrHit();
        }
        else if(dealerFaceUpValue >= 3 && dealerFaceUpValue <= 6){
            tellUserToSplit();
        }
        else{
            tellUserToHit();
        }
        return true;
     }
     else if(firstCard.getValue() == 7 && secondCard.getValue() == 7){
        if(dealerFaceUpValue <= 7){
            tellUserToSplit();
        }
        else{
            tellUserToHit();
        }
        return true;
     }
     else if(firstCard.getValue() == 8 && secondCard.getValue() == 8){
        tellUserToSplit();
        return true;
     }
     else if(firstCard.getValue() == 9 && secondCard.getValue() == 9){
        if(dealerFaceUpValue <= 6 || (dealerFaceUpValue == 8 || dealerFaceUpValue == 9)){
            tellUserToSplit();
        }
        else{
            tellUserToStand();
        }
        return true;
     }
     else if(firstCard.getValue() == 10 && secondCard.getValue() == 10){
        tellUserToStand();
        return true;
     }

     return false;
 }

 void MainWindow::tellUserToHit(){
     helpwidget->setText("HIT");
     helpwidget->show();
 }

 void MainWindow::tellUserToStand(){
     helpwidget->setText("STAND");
     helpwidget->show();
 }

 void MainWindow::tellUserToDoubleDownOrHit(){
     helpwidget->setText("DOUBLE DOWN or HIT");
     helpwidget->show();
 }

 void MainWindow::tellUserToDoubleDownOrStand(){
     helpwidget->setText("DOUBLE DOWN or STAND");
     helpwidget->show();
 }

 void MainWindow::tellUserToSplit(){
     helpwidget->setText("SPLIT");
     helpwidget->show();
 }

 void MainWindow::tellUserToSplitOrHit(){
     helpwidget->setText("SPLIT or HIT");
     helpwidget->show();
 }

 void MainWindow::insurance() {
     int sideBet = model.getBet() / 2.0;

     if(model.getDealerTotal() + model.faceDownValue() == 21) {
        model.adjustBankTotal(2 * sideBet);
        ui->insuranceResult->setText("INSURANCE WON");
     } else {
        model.adjustBankTotal(-sideBet);
        ui->insuranceResult->setText("INSURANCE LOST");
     }

     ui->insuranceResult->setVisible(true);

     ui->insuranceButton->setVisible(false);
     ui->doubleDownButton->setVisible(false);
     updateBankDisplay();
 }

 void MainWindow::handleLessonSelect() {
     ui->dealerHand->clearAllCards();
     ui->playerHand->clearAllCards();
     ui->stackedWidget->setCurrentWidget(ui->game);
     hideAllUI();
     ui->dealerScore->setVisible(false);
     ui->playerScore->setVisible(false);
     ui->bank->setVisible(false);
     ui->currentBet->setVisible(false);
     ui->tutorialLabel->setVisible(true);
     if (sender() == ui->lessonOne) {
        ui->nextLessonOne->setVisible(true);
     }
     if (sender() == ui->lessonTwo) {
        ui->nextLessonTwo->setVisible(true);
     }
     if (sender() == ui->lessonThree) {
        ui->nextLessonThree->setVisible(true);
     }
     ui->tutorialTextLabel->setVisible(true);
     ui->tutorialTextLabel->setText("ℹ️ Tutorial Selected\n"
                                    "To progess through the tutorial please click the\n"
                                    "'Next Step' button.");
 }

 void MainWindow::nextLessonOneStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->dealerHand->addDealerCard(":/cards/7D.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/QC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");

        ui->tutorialTextLabel->setText("ℹ️ Card Values\n"
                                       "1) Number cards (2-10) are worth their face value.\n"
                                       "2) Face cards (Jack, Queen, King) are each worth 10.\n"
                                       "3) Aces can be worth 1 or 11, whichever is more beneficial.\n"
                                       "🤓 Example\n"
                                       "Dealer Hand) 7D(7) + 10C(10) = 17\n"
                                       "Player Hand) QC(10) + AC(1/11) = 11 or 21");
        break;
     case 2:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->addPlayerCard(":/cards/5S.png");

        ui->tutorialTextLabel->setText("ℹ️ Game Setup\n"
                                       "1) Each player, including the dealer, is dealt two cards.\n"
                                       "Players' cards are usually face-up.\n"
                                       "2) While one of the dealer's cards is face-up (known as the\n"
                                       "upcard) and the other face-down (the hole card).");
        break;
     case 3:
        ui->playerHand->addPlayerCard(":/cards/3H.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");

        ui->tutorialTextLabel->setText("ℹ️ Player Turns\n"
                                       "1) Players decide whether to 'hit' (take another card) or\n"
                                       "'stand' (keep their current hand).\n"
                                       "2) Players can continue to hit as many times as they want,\n"
                                       "but if the total value of their hand exceeds 21, they 'bust'\n"
                                       "and lose the game.");
        break;
     case 4:
        ui->dealerHand->flipDealerCard(":/cards/7C.png");

        ui->tutorialTextLabel->setText("ℹ️ Dealer Turns\n"
                                       "1) The dealer reveals their hole card.\n"
                                       "2) The dealer must draw on 16 or under and must stand on 17\n"
                                       "or over.");
        break;
     case 5:
        ui->tutorialTextLabel->setText("ℹ️ Winning\n"
                                       "1) If a player's hand is closer to 21 than the dealer's\n"
                                       "without busting, the player wins.\n"
                                       "2) If the dealer busts, all remaining players win.\n"
                                       "3) If both the player and dealer have the same hand value, it's\n"
                                       "a 'push' (a tie), and the player's bet is returned.");
        break;
     case 6:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/KC.png");

        ui->tutorialTextLabel->setText("ℹ️ Special Moves\n"
                                       "♦️ Blackjack\n"
                                       "If a player is dealt an Ace and a 10-value card as their initial\n"
                                       "two cards, it's called a 'Blackjack' or 'natural' and the player\n"
                                       "wins unless dealer also has Blackjack.");
        break;
     case 7:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/3D.png");
        ui->playerHand->splitPlayerCards();

        ui->tutorialTextLabel->setText("ℹ️ Special Moves\n"
                                       "♥️ Split\n"
                                       "If a player's initial two cards are of the same value, they can\n"
                                       "choose to split them into two separate hands, each with its bet.");
        break;
     case 8:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/8D.png");
        ui->playerHand->doubleDownPlayerCard(":/cards/QC.png");

        ui->tutorialTextLabel->setText("ℹ️ Special Moves\n"
                                       "♠️ Double Down\n"
                                       "After receiving the first two cards, a player can choose\n"
                                       "to double their original bet in exchange for committing\n"
                                       "to stand after receiving one more card.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->nextLessonOne->setVisible(false);

        ui->tutorialTextLabel->setText("ℹ️ 'Rules of Blackjack' Tutorial Complete\n"
                                       "Please click the 'Main Menu' button to return back\n"
                                       "to the home screen.");
        return;
     }
     // Increment the tutorial step
     tutorialStep++;
 }

 void MainWindow::nextLessonTwoStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->playerHand->addPlayerCard(":/cards/KC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->splitPlayerCards();
        ui->playerHand->addPlayerCard(":/cards/7C.png");

        ui->tutorialTextLabel->setText("ℹ️ Hit or Stand\n"
                                       "1) If your hand is at a total of 17 or higher,\n"
                                       "it's generally advisable to stand to avoid the\n"
                                       "risk of busting.\n"
                                       "2) When your hand is below a certain total\n"
                                       "(usually 11), it is often recommended to hit\n"
                                       "to improve your hand.");
        break;
     case 2:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/6H.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->doubleDownPlayerCard(":/cards/AC.png");
        ui->dealerHand->addDealerCard(":/cards/QH.png");
        ui->dealerHand->addDealerCard(":/cards/6C.png");

        ui->tutorialTextLabel->setText("ℹ️ Doubling Down\n"
                                       "Double down when you have a hand value of\n"
                                       "10 or 11, especially when the dealer's face-up\n"
                                       "card is weaker.");
        break;
     case 3:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->splitPlayerCards();

        ui->tutorialTextLabel->setText("ℹ️ Splitting Pairs\n"
                                       "1) Split if you get a pair of Aces.\n"
                                       "2) Split if you get a pair of 8s.\n"
                                       "3) Avoid spitting 10s, value of 20 is strong.");
        break;
     case 4:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/JC.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");

        ui->tutorialTextLabel->setText("ℹ️ Insurance\n"
                                       "Avoid taking insurance bets, as they statistically favor\n"
                                       "the house in the long run.");
        break;
     case 5:
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");

        ui->tutorialTextLabel->setText("ℹ️ Soft Hands\n"
                                       "With a soft hand (an Ace and another card), it's often\n"
                                       "safe to hit without the risk of busting since Ace is 1 or 11.");
        break;
     case 6:
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/AD.png");

        ui->tutorialTextLabel->setText("ℹ️ Dealer's Up Card\n"
                                       "Pay attention to the dealer's up card. Adjust your\n"
                                       "strategy based on their visible card, as it gives insight\n"
                                       "into the likelihood of their final hand.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->nextLessonTwo->setVisible(false);

        ui->tutorialTextLabel->setText("ℹ️ 'Basic Strategies' Tutorial Complete\n"
                                       "Please click the 'Main Menu' button to return back\n"
                                       "to the home screen.");
        return;
     }
     // Increment the tutorial step
     ++tutorialStep;
 }


 void MainWindow::nextLessonThreeStep() {
     // Displays and updates UI and scene based on current tutorial step
     switch (tutorialStep) {
     case 1:
        ui->playerHand->addPlayerCard(":/cards/2C.png");
        ui->playerHand->addPlayerCard(":/cards/3C.png");
        ui->playerHand->addPlayerCard(":/cards/4C.png");
        ui->playerHand->addPlayerCard(":/cards/5C.png");
        ui->playerHand->addPlayerCard(":/cards/6C.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");
        ui->playerHand->addPlayerCard(":/cards/8C.png");
        ui->playerHand->addPlayerCard(":/cards/9C.png");
        ui->playerHand->addPlayerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");

        ui->tutorialTextLabel->setText("ℹ️ Card Counting\n"
                                       "Keep track of the ratio of high to low-value cards\n"
                                       "remaining in the deck. Adjust your bets and actions\n"
                                       "accordingly\n"
                                       "🤓 Example\n"
                                       "Use the Hi-Lo counting system where cards 2-6 have\n"
                                       "a value of +1, 7-9 have a value of 0, and 10-Ace have\n"
                                       "a value of -1. Keep a running count as the cards are dealt.");
        break;
     case 2:
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/facedown.png");
        ui->dealerHand->addDealerCard(":/cards/10C.png");
        ui->playerHand->addPlayerCard(":/cards/7C.png");
        ui->playerHand->addPlayerCard(":/cards/4H.png");

        ui->tutorialTextLabel->setText("ℹ️ Basic Strategy Variations\n"
                                       "Depending on the specific rules of the game, there\n"
                                       "might be variations in basic strategy. Learn and apply\n"
                                       "these variations for optimal play.\n"
                                       "🤓 Example\n"
                                       "Modify basic strategy based on the count. For a positive\n"
                                       "count, consider doubling down on 11 against a dealer's 10,\n"
                                       "even if basic strategy suggests otherwise.");
        break;
     case 3:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->dealerHand->addDealerCard(":/cards/AH.png");
        ui->dealerHand->addDealerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AS.png");
        ui->playerHand->addPlayerCard(":/cards/2H.png");

        ui->tutorialTextLabel->setText("ℹ️ Side Counting\n"
                                       "Track specific cards, such as Aces, separately from the\n"
                                       "overall count to gain a more accurate picture of the deck\n"
                                       "composition.\n"
                                       "🤓 Example\n"
                                       "Look at what cards are on the table like in this example\n"
                                       "three aces are out at play.");
        break;
     case 4:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->playerHand->addPlayerCard(":/cards/AH.png");
        ui->playerHand->addPlayerCard(":/cards/AC.png");
        ui->playerHand->addPlayerCard(":/cards/AD.png");
        ui->playerHand->addPlayerCard(":/cards/AS.png");

        ui->tutorialTextLabel->setText("ℹ️ Beware\n"
                                       "It's essential to note that while basic strategies can\n"
                                       "improve your game, advanced strategies like card counting\n"
                                       "may be restricted or disallowed by different instituions,\n"
                                       "and attempting them could lead to being barred from play.\n"
                                       "Always be aware of and adhere to different rules and\n"
                                       "regulations.");
        break;
     default:
        ui->dealerHand->clearAllCards();
        ui->playerHand->clearAllCards();
        ui->nextLessonThree->setVisible(false);

        ui->tutorialTextLabel->setText("ℹ️ 'Advanced Strategies' Tutorial Complete\n"
                                       "Please click the 'Main Menu' button to return back\n"
                                       "to the home screen.");
        return;
     }
     // Increment the tutorial step
     ++tutorialStep;
 }

 void MainWindow::speechModeClicked(bool toggled) {
     if (toggled) speech = true;
     else         speech = false;
     qDebug() << speech;
 }

 void MainWindow::sayObjectName() {
     if(speech) {
        QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
        const QString name = clickedButton->objectName();
        if (name.startsWith("nextLesson"))  say.say("Next Step");
        else if (name.startsWith("main"))   say.say("Main Menu");
        else if (name.endsWith("Button"))   say.say(name.left(name.length() - 6));
        else                                say.say(name);
     }
 }

 void MainWindow::settingsClicked() {
     ui->stackedWidget->setCurrentWidget(ui->settingsMenu);
 }

 void MainWindow::showSheet() {
     QDialog* sheetDialog = new QDialog(this);
     sheetDialog->setWindowTitle("Sheet Help Popup");
     QVBoxLayout* layout = new QVBoxLayout(sheetDialog);
     QLabel* tutorialLabel = new QLabel(sheetDialog);
     tutorialLabel->setPixmap(QPixmap(sheetLocation));
     layout->addWidget(tutorialLabel);
     sheetDialog->exec();
 }

 void MainWindow::displayAdvice() {
     showATip();
     tipTimer.start(4000);
     qDebug() << "ran displayAdvice";
 }

 void MainWindow::hideTip(){
     helpwidget->hide();
     qDebug() << "ran hideTip";
 }

 void MainWindow::lostGame() {
     ui->newGame->setVisible(true);
     ui->outcome->setVisible(true);
     ui->outcome->setText("OUT OF FUNDS!");
 }

 void MainWindow::newGame() {
     model = Model();
     ui->playerHand->clearDiscardPile();
     ui->dealerHand->clearDiscardPile();
     setupDeal();
 }

void MainWindow::speechModeSettingAdjusted(const QString &arg1)
{
    speechModeClicked(arg1 == "On");
}

void MainWindow::themeChanged(const QString &arg1) {
    if (arg1 == "Casino") {
        ui->centralwidget->setStyleSheet("background-color: rgb(46, 77, 62)");
        sheetLocation = ":/cards/sheetCasino.png";
    }
    else if (arg1 == "Modern") {
        ui->centralwidget->setStyleSheet("background-color: #1e1e21");
        sheetLocation = ":/cards/sheetModern.png";
    }
}
