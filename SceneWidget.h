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

private:
    /// @brief Defines a Box2D world.
    b2World world;

    /// @brief Handles periodically updating the world.
    QTimer timer;

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

    /// @brief Whether text box is enabled or not.
    bool textBoxEnabled;

    /// @brief Text to display within text box.
    QString textBoxString;

    /// @brief Stores blackground image of a blackjack table.
    QImage backgroundImage;

    /// @brief Stores all dealer bodies that are created.
    QVector<b2Body*> dealerBodies;

    /// @brief Stores all player bodies that are created.
    QVector<b2Body*> playerBodies;

    /// @brief Stores all dealer images that are added.
    QVector<QImage> dealerImages;

    /// @brief Stores all player images that are added.
    QVector<QImage> playerImages;

    /// @brief Helper function to create a card body
    /// @param x coordinate for card body location
    /// @param y coordinate for card body location
    b2Body* createCardBody(float x, float y);

protected:
    /// @brief Overrides the paintEvent function to customize the rendering.
    void paintEvent(QPaintEvent *) override;

    void resizeEvent(QResizeEvent *event) override;

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
    void enableTextBox();

    void setTextBox(const QString &text);

private slots:
    /// @brief Utilized with a timer to update the world and card physics.
    void updateWorld();
};

#endif // SCENEWIDGET_H
