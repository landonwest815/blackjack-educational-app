#include "SceneWidget.h"

/**
 * Author(s):     Team Six of Hearts:
 *                Ryan Nguyen (Taizuki), John Nguyen (johnguyn),
 *           	  Christian Hartman (Is-This-One-Avaiable), Caleb Funk (Funkyc02),
 *                Landon West (landonwest815), and Pablo Arancibia-Bazan (Pablo23117)
 * Course:        CS 3505, University of Utah, School of Computing
 * Assignment:    A9 - Educational App
 * File Contents:
 * Utilizes Box2D in order to create a world populated with bodies that are cards
 * which all come together to create and display a game of blackjack.
 */

SceneWidget::SceneWidget(QWidget *parent)
    : QWidget(parent),
    world(b2Vec2(0.0f, 0.0f)),
    timer(this),
    split(false),
    nextSplit(false),
    splitCountBool(true),
    splitCount(0),
    doubleDown(false) {

    // Defines the low ground body and its various settings
    b2BodyDef lowGroundDef;
    lowGroundDef.position.Set(0.0f, 30.0f); // Y-coordinate for the low ground
    b2Body* lowGround = world.CreateBody(&lowGroundDef);
    b2PolygonShape lowGroundShape;
    lowGroundShape.SetAsBox(50.0f, 1.0f); // Low ground box size
    lowGround->CreateFixture(&lowGroundShape, 0.0f);

    // Defines the high ground body and its various settings
    b2BodyDef highGroundDef;
    highGroundDef.position.Set(0.0f, 10.0f); // Y-coordinate for the high ground
    b2Body* highGround = world.CreateBody(&highGroundDef);
    b2PolygonShape highGroundShape;
    highGroundShape.SetAsBox(50.0f, 1.0f); // High ground box size
    highGround->CreateFixture(&highGroundShape, 0.0f);

    // Assists in updating the world based on a timer
    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);
    timer.start(10);
}

void SceneWidget::addDealerCard(const QString& imagePath) {
    // Creates body of dealer card and adds to the associated vector
    b2Body* dealerBody = nullptr;
    dealerBody = createCardBody(-6.0, this->height() / 30.0); // Initial position of dealer body
    dealerBodies.push_back(dealerBody);

    // Creates image of dealer card and adds to the associated vector
    QImage dealerImage(imagePath);
    dealerImage = dealerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    dealerImages.push_back(dealerImage);
}

void SceneWidget::addPlayerCard(const QString& imagePath) {
    if (splitCountBool) {
        splitCount++;
    }

    // Creates body of player card and adds to the associated vector
    b2Body* playerBody = nullptr;
    playerBody = createCardBody(12.0, this->height() / 200.0); // Initial position of player body
    playerBodies.push_back(playerBody);

    // Creates image of player card and adds to the associated vector
    QImage playerImage(imagePath);
    playerImage = playerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    if (doubleDown) {
        playerImage = playerImage.transformed(QTransform().rotate(-90));
    }
    playerImages.push_back(playerImage);
}

b2Body* SceneWidget::createCardBody(float x, float y) {
    // Defines the properties of the Box2D card body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);

    b2Body* body = world.CreateBody(&bodyDef); // Create body in the world

    // Defines the shape of the dynamic box
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the properties of the Box2D fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.groupIndex = -1;
    body->CreateFixture(&fixtureDef);

    return body;
}

void SceneWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Get positions
    int rectWidth = this->width() / 2;
    int rectHeight = this->height();
    int left = (this->width() - rectWidth) / 2;
    int top = (this->height() - rectHeight) / 2;

    // Use the gradient as the brush
    painter.setBrush(QBrush(QColor::fromRgb(62, 62, 66)));

    // Set the pen to NoPen to avoid drawing the widget's border
    painter.setPen(Qt::NoPen);

    // You can adjust the corner radius by changing the '10'
    painter.drawRoundedRect(left, top, rectWidth, rectHeight, 10, 10);

    // Draws the card bodies of the dealer
    for (int i = 0; i < dealerBodies.size(); ++i) {
        b2Vec2 position = dealerBodies[i]->GetPosition();

        // Sets the position where each dealer card body added will fall
        painter.drawImage((position.x * 0.5 * 145) - (i * 40), (position.y * 0.5 * 10), dealerImages[i]);
    }

    // Draws the card bodies of the player
    for (int i = 0; i < playerBodies.size(); ++i) {
        b2Vec2 position = playerBodies[i]->GetPosition();
        float positionLeftX = position.x * 0.5 * 325;
        float positionRightX = position.x * 0.5 * 600;
        float positionY = position.y * 0.5 * 70;

        // Sets the position where each player card body added will fall and will change is split is true
        if (split) {
            if (i == 0) {
                painter.drawImage(positionLeftX, positionY, playerImages[0]);
            }
            if (i == 1) {
                painter.drawImage(positionRightX, positionY, playerImages[1]);
            }
            if (i >= 2 && !nextSplit) {
                painter.drawImage(positionLeftX + (i * 30) - 30, positionY, playerImages[i]);
            }
            if (i >= 2 && i < splitCount && nextSplit) {
                painter.drawImage(positionLeftX + (i * 30) - 30, positionY, playerImages[i]);
            }
            if (i >= 2 && i >= splitCount && nextSplit) {
                painter.drawImage(positionRightX + (i * 30) - ((splitCount - 1) * 30), positionY, playerImages[i]);
            }
        } else {
            painter.drawImage(positionLeftX + (i * 40), positionY, playerImages[i]);
        }
    }

    painter.end();
}

void SceneWidget::updateWorld() {
    // Updates all the card bodies of the dealer
    for (int i = 0; i < dealerBodies.size(); ++i) {
        dealerBodies[i]->SetLinearVelocity(b2Vec2(14.0f, 0.0f)); // Velocity direction right
        if (dealerBodies[i]->GetPosition().x * 50 >= this->width() / 2.4) {
            dealerBodies[i]->SetLinearVelocity(b2Vec2_zero);
        }
    }

    // Updates all the card bodies of the player
    for (int i = 0; i < playerBodies.size(); ++i) {
        playerBodies[i]->SetLinearVelocity(b2Vec2(-6.0f, 0.0f)); // Velocity direction left
        if (playerBodies[i]->GetPosition().x * 50 <= this->width() / 12.0) {
            playerBodies[i]->SetLinearVelocity(b2Vec2_zero);
        }
    }

    world.Step(1.0 / 60.0, 6, 2);
    update();
}

void SceneWidget::clearAllCards() {
    // Resets associated variables
    splitCount = 0;
    split = false;
    nextSplit = false;
    splitCountBool = true;
    // Removes all dealer bodies from the world
    for (b2Body* dealerBody : dealerBodies) {
        world.DestroyBody(dealerBody);
    }
    // Removes all player bodies from the world
    for (b2Body* playerBody : playerBodies) {
        world.DestroyBody(playerBody);
    }
    // Clears out all associated dealer and player vectors
    dealerBodies.clear();
    playerBodies.clear();
    dealerImages.clear();
    playerImages.clear();
}

void SceneWidget::splitPlayerCards() {
    // Allows the player card bodies to be split when only two exist
    if (playerBodies.size() == 2) {
        b2Vec2 newPosition(6.0, this->height() / 200.0);
        playerBodies[0]->SetTransform(newPosition, 0.0f);
        playerBodies[1]->SetTransform(newPosition, 0.0f);
        split = true;
    }
}

void SceneWidget::nextSplitHand() {
    nextSplit = true;
    splitCountBool = false;
}

void SceneWidget::flipDealerCard(const QString& imagePath) {
    // Update the image of dealer hidden card
    if (!dealerBodies.empty()) {
        QImage dealerImage(imagePath);
        dealerImage = dealerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
        dealerImages[0] = dealerImage;
    }
}

void SceneWidget::doubleDownPlayerCard(const QString& imagePath) {
    // Inserts a single sideways card to represent double down
    doubleDown = true;
    addPlayerCard(imagePath);
    doubleDown = false;
}

void SceneWidget::resizeEvent(QResizeEvent *event) {
    // Call the base class implementation
    QWidget::resizeEvent(event);

    // Update the scale of the card images
    for (QImage &image : dealerImages) {
        image = image.scaled(this->width() / 2, this->height() / 2, Qt::KeepAspectRatio);
    }

    for (QImage &image : playerImages) {
        image = image.scaled(this->width() / 2, this->height() / 2, Qt::KeepAspectRatio);
    }

    // Repaint the widget with the new sizes
    update();
}
