/**
 * Author(s):     Team Six of Hearts:
 *                Ryan Nguyen (Taizuki), John Nguyen (johnguyn),
 *           	  Christian Hartman (Is-This-One-Avaiable), Caleb Funk (Funkyc02),
 *                Landon West (landonwest815), and Pablo Arancibia-Bazan (Pablo23117)
 * Course:        CS 3505, University of Utah, School of Computing
 * Assignment:    A9 - Educational App
 * File Contents:
 * This project helps to demonstrate some C++ conventions in regards
 * to the topic of a SceneWidget class which has been split into a header
 * file. It assists in declarations of the SceneWidget object, functions, etc.
 */

#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <Box2D/Box2D.h>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <random>

/**
 * @brief The SceneWidget class handles Box2D functionality and assists in
 * displaying the blackjack table and the physics of adding and removing cards.
 */
class SceneWidget : public QWidget {
    Q_OBJECT
public:
    /// @brief Constructor for the SceneWidget class.
    /// @param parent The parent QWidget.
    explicit SceneWidget(QWidget *parent = nullptr);

protected:
    /// @brief Overrides the paintEvent function to customize the rendering.
    void paintEvent(QPaintEvent *) override;

    /// @brief Overrides the resizeEvent function to customize the sizing.
    void resizeEvent(QResizeEvent *event) override;

private:

    // DATA

    /// @brief Defines a Box2D world.
    b2World world;

    /// @brief Helper function to create a card body
    /// @param x coordinate for card body location
    /// @param y coordinate for card body location
    b2Body* createCardBody(float x, float y);

    /// @brief Handles periodically updating the world.
    QTimer timer;

    /// @brief Handles falling tokens animation
    QTimer coinTimer;

    /// @brief Bound for keeping player discard pile on screen.
    float playerDiscardBound;

    /// @brief Bound for keeping dealer discard pile on screen.
    float dealerDiscardBound;

    /// @brief Tracks the time to allow for shaking of cards.
    float elapsedTime;

    /// @brief Whether the player body cards are split.
    bool split;

    /// @brief Whether to add player body cards to the second split hand.
    bool nextSplit;

    /// @brief Whether or not to stop the split count.
    bool splitCountBool;

    /// @brief Count of the cards in the first split hand.
    int splitCount;

    /// @brief Whether the player body cards are double down.
    bool doubleDown;

    /// @brief Whether card shaking is enabled when a player/dealer busts.
    bool isShakingEnabled;

    /// @brief Whether this SceneWidget is the player hand or not.
    bool isPlayerHand;

    /// @brief Whether the player just won a hand.
    bool playerWon;

    /// @brief Whether text box is enabled or not.
    bool textBoxEnabled;

    /// @brief Text to display within text box.
    QString textBoxString;

    /// @brief Stores all dealer bodies that are created.
    QVector<b2Body*> dealerBodies;

    /// @brief Stores all dealer discard bodies that are created.
    QVector<b2Body*> dealerDiscardBodies;

    /// @brief Stores all dealer images that are added.
    QVector<QImage> dealerImages;

    /// @brief Stores all dealer discard body images.
    QVector<QImage> dealerDiscardImages;

    /// @brief Stores all player bodies that are created.
    QVector<b2Body*> playerBodies;

    /// @brief Stores all player discard bodies that are created.
    QVector<b2Body*> playerDiscardBodies;

    /// @brief Stores all player images that are added.
    QVector<QImage> playerImages;

    /// @brief Stores all player discard body images.
    QVector<QImage> playerDiscardImages;

    /// @brief Stores all the chip bodies.
    QVector<b2Body*> chipBodies;

    /// @brief Stores all the chip images.
    QVector<QImage> chipImages;

    /// @brief Holds the png of a facedown card.
    QImage faceDownImage;

    /// @brief Helps generate random numbers
    std::mt19937 gen;

    /// @brief Helps set a bound for random numbers
    std::uniform_real_distribution<> dis;

    /// @brief Applies a tilt to a card body
    /// @param body The card to adjust
    /// @param angle The angle to adjust to
    void applyTiltAngle(b2Body* body, float angle);

    /// @brief Creates a physics body for a chip
    /// @param x The x coordinate to place it at
    /// @param y The y coordinate to place it at
    b2Body* createChipBody(float x, float y);

    /// @brief Adds a Chip to the program
    /// @param randomly generated x coordinate to add the chip at
    void addChip(float x);

    /// @brief Spawns a Chip into the program at a random location
    void spawnChip();

    /// @brief Draws the tokens currently in the world.
    void drawTokens(QPainter &painter);

    /// @brief Draws the card area
    void drawCardArea(QPainter &painter);

    /// @brief Draws the cards currently in your hand
    void drawCards(QPainter &painter);

    /// @brief Draws the cards in the dealer location
    void drawDealerCards(QPainter &painter);

    /// @brief Draws the cards in the player location
    void drawPlayerCards(QPainter &painter);

    /// @brief Draws the cards currently in the discard pile
    void drawDiscardPile(QPainter &painter);

    /// @brief Draws the player discard pile
    void drawPlayerDiscardPile(QPainter &painter);

    /// @brief Draws the dealer discard pile
    void drawDealerDiscardPile(QPainter &painter);

    /// @brief Draws the deck to the side
    void drawDeck(QPainter &painter);

    /// @brief Updates the velocity of the cards so they can move to the correct position fast
    void updateCardBodiesVelocity(QVector<b2Body*>& bodies, float velocityX, float boundaryX);

    /// @brief Updates the velocity of the discarded cards so they can move to the correct locations
    void updateDiscardedCardBodiesVelocity(QVector<b2Body*>& bodies, float velocityX, float boundaryX);

    /// @brief Updates the angle for cards when they should be shaking
    void updateTiltAngleForShaking();

    /// @brief Updates everything drawn in the world
    void updateWorld();

public slots:

    /// @brief Draws a new dealer card with given image.
    /// @param imagePath String path of image for dealer card to display.
    void addDealerCard(const QString& imagePath);

    /// @brief Draws a new player card with given image.
    /// @param imagePath String path of image for player card to display.
    void addPlayerCard(const QString& imagePath);

    /// @brief Clears all the player and dealer cards.
    void clearAllCards();

    /// @brief Redraws player cards to now be split when two cards are present.
    void splitPlayerCards();

    /// @brief Enables drawing the second split player hand.
    void nextSplitHand();

    /// @brief Draws a new dealer card with given image replacing the hidden dealer card.
    /// @param imagePath String path of image for new replacement dealer card to display.
    void flipDealerCard(const QString& imagePath);

    /// @brief Draws a new player card with given image on its side to represent double down.
    /// @param imagePath String path of image for player card to display.
    void doubleDownPlayerCard(const QString& imagePath);

    /// @brief Draws a text box that contains the text the parameter is given.
    /// @param text String to display in the text box.
    void drawTextBox(const QString &text);

    /// @brief Allows text box to be displayed.
    void setTextBoxEnabled();

    /// @brief Sets the text displayed in the text box.
    /// @param text String to display in the text box.
    void setTextBoxText(const QString &text);

    /// @brief Sets whether the cards should shake or not.
    void setShakingEnabled(bool);

    /// @brief Clears out the discard pile
    void clearDiscardPile();

    /// @brief Sets whether this SceneWidget is the player hand.
    void setIsPlayerHand(bool);

    /// @brief Sets whether the player just won a hand.
    void setPlayerWon(bool);
};

#endif // SCENEWIDGET_H
