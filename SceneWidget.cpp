#include "SceneWidget.h"
#include <Random>

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

std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 gen;       // Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis;

SceneWidget::SceneWidget(QWidget *parent)
    : QWidget(parent),
    world(b2Vec2(0.0f, 0.0f)),
    timer(this),
    coinTimer(this),
    split(false),
    nextSplit(false),
    splitCountBool(true),
    splitCount(0),
    doubleDown(false),
    textBoxEnabled(false),
    textBoxString(""),
    isShakingEnabled(false),
    playerDiscardBound(0.75),
    isPlayerHand(true) {

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
    connect(&coinTimer, &QTimer::timeout, this, &SceneWidget::spawnCoin);
    timer.start(10);
    coinTimer.start(150);

    srand(static_cast<unsigned int>(time(nullptr)));    

    QImage image(":/cards/facedown.png");
    faceDownImage = image;

    dealerDiscardBound = (this->width() / 80.0) - 1.0;
}

void SceneWidget::addDealerCard(const QString& imagePath) {
    // Creates body of dealer card and adds to the associated vector
    b2Body* dealerBody = nullptr;
    dealerBody = createCardBody(2.0, this->height() / 30.0); // Initial position of dealer body
    dealerBodies.push_back(dealerBody);

    // Creates image of dealer card and adds to the associated vector
    QImage dealerImage(imagePath);
    dealerImage = dealerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    dealerImages.push_back(dealerImage);

    isPlayerHand = false;
}

void SceneWidget::addPlayerCard(const QString& imagePath) {
    if (splitCountBool) {
        splitCount++;
    }

    // Creates body of player card and adds to the associated vector
    b2Body* playerBody = nullptr;
    playerBody = createCardBody(this->width() / 200.0, this->height() / 200.0); // Initial position of player body
    playerBodies.push_back(playerBody);

    // Creates image of player card and adds to the associated vector
    QImage playerImage(imagePath);
    playerImage = playerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    if (doubleDown) {
        playerImage = playerImage.transformed(QTransform().rotate(-90));
    }
    playerImages.push_back(playerImage);

    isPlayerHand = true;
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
    painter.setRenderHint(QPainter::Antialiasing); // For smoother edges

    for (int i = 0; i < coinBodies.size(); ++i) {
        b2Vec2 position = coinBodies[i]->GetPosition();
        QImage &coinImage = coinImages[i];
        painter.drawImage(QRectF(position.x, position.y, coinImage.width(), coinImage.height()), coinImage);
    }

    /// Set the pen width
    int penWidth = 4;

    // Calculate the adjusted positions and dimensions
    int rectWidth = this->width() / 2 - penWidth;
    int rectHeight = this->height() - penWidth;
    int left = (this->width() - rectWidth) / 2;
    int top = (this->height() - rectHeight) / 2;

    // Set the pen and brush
    QPen outlinePen(Qt::white, penWidth, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter.setPen(outlinePen);
    painter.setBrush(QBrush(QColorConstants::Transparent));

    // Draw the rounded rectangle
    painter.drawRoundedRect(left, top, rectWidth, rectHeight, 10, 10);

    if (!isPlayerHand) {
    // Draws the card bodies of the dealer
    for (int i = 0; i < dealerBodies.size(); ++i) {

        b2Vec2 position = dealerBodies[i]->GetPosition();
        float positionLeftX = position.x * 0.5 * 145;
        float positionY = position.y * 0.5 * 10;

        QImage &image = dealerImages[i];
        QRect cardRect;

        cardRect = QRect(positionLeftX - (i * 40), positionY, image.width(), image.height());

        if (isShakingEnabled) {
            painter.save(); // Save the current state of the painter

            // Translate the painter to the center of the last card
            painter.translate(cardRect.left() + image.width() / 2, cardRect.top() + image.height() / 2);

            // Apply rotation
            float angle = dealerBodies[i]->GetAngle();
            painter.rotate(angle * 180.0 / M_PI * 0.25); // Convert radians to degrees

            // Draw the last card image, taking into account the rotation
            painter.drawImage(QRect(-image.width() / 2, -image.height() / 2, image.width(), image.height()), image);

            painter.restore(); // Restore the painter's state
        } else {
            // Draw the card normally if it's not the last card or shaking is not enabled
            painter.drawImage(cardRect, image);
        }
    }
    }
    else {

    // Draws the card bodies of the player
    for (int i = 0; i < playerBodies.size(); ++i) {
        b2Vec2 position = playerBodies[i]->GetPosition();
        float positionLeftX = position.x * 0.5 * 325;
        float positionRightX = position.x * 0.5 * 600;
        float positionY = position.y * 0.5 * 70;

        QImage &image = playerImages[i];
        QRect cardRect;

        if (split) {
            if (i == 0) {
                cardRect = QRect(positionLeftX, positionY, image.width(), image.height());
            } else if (i == 1) {
                cardRect = QRect(positionRightX, positionY, image.width(), image.height());
            } else if (i >= 2) {
                if (!nextSplit || i < splitCount) {
                    cardRect = QRect(positionLeftX + (i * 30) - 30, positionY, image.width(), image.height());
                } else {
                    cardRect = QRect(positionRightX + (i * 30) - ((splitCount - 1) * 30), positionY, image.width(), image.height());
                }
            }
        } else {
            cardRect = QRect(positionLeftX + (i * 40), positionY, image.width(), image.height());
        }

       // if (isShakingEnabled && i == playerBodies.size() - 1) {
        if (isShakingEnabled) {
            painter.save(); // Save the current state of the painter

            // Translate the painter to the center of the last card
            painter.translate(cardRect.left() + image.width() / 2, cardRect.top() + image.height() / 2);

            // Apply rotation
            float angle = playerBodies[i]->GetAngle();
            painter.rotate(angle * 180.0 / M_PI * 0.25); // Convert radians to degrees

            // Draw the last card image, taking into account the rotation
            painter.drawImage(QRect(-image.width() / 2, -image.height() / 2, image.width(), image.height()), image);

            painter.restore(); // Restore the painter's state
        } else {
            // Draw the card normally if it's not the last card or shaking is not enabled
            painter.drawImage(cardRect, image);
        }
    }
    }

     //Draws the card bodies of the player discard
    if (isPlayerHand) {
        for (int i = 0; i < playerDiscardBodies.size(); ++i) {
            b2Vec2 position = playerDiscardBodies[i]->GetPosition();
            float positionLeftX = position.x * 0.5 * 325;
            float positionY = position.y * 0.5 * 70;

            QImage &image = playerDiscardImages[i];
            QRect cardRect;

            if (i == 0)
                cardRect = QRect(positionLeftX - 10, positionY, image.width(), image.height());
            else if (i == 1)
                cardRect = QRect(positionLeftX - 5, positionY, image.width(), image.height());
            else
                cardRect = QRect(positionLeftX, positionY, image.width(), image.height());

            painter.drawImage(cardRect, image);
        }
    }
    else {
        //Draws the card bodies of the player discard
        for (int i = 0; i < dealerDiscardBodies.size(); ++i) {
            b2Vec2 position = dealerDiscardBodies[i]->GetPosition();
            float positionLeftX = position.x * 0.5 * 145;
            float positionY = position.y * 0.5 * 10;

            QImage &image = dealerDiscardImages[i];
            QRect cardRect;

            if (i == 0)
                cardRect = QRect(positionLeftX - 10, positionY, image.width(), image.height());
            else if (i == 1)
                cardRect = QRect(positionLeftX - 5, positionY, image.width(), image.height());
            else
                cardRect = QRect(positionLeftX, positionY, image.width(), image.height());

            painter.drawImage(cardRect, image);
        }
    }


        for (int i = 0; i < 3; i++) {
            float positionY = (this->height() / 200.0) * 0.5 * 70;
            QImage image = faceDownImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
            QRect cardRect;

            float positionX = 0.0;

            if (isPlayerHand) positionX = this->width() - 200;
            else              positionX = 110;

            if (i == 0)
                cardRect = QRect(positionX - 10, positionY, image.width(), image.height());
            else if (i == 1)
                cardRect = QRect(positionX - 5, positionY, image.width(), image.height());
            else
                cardRect = QRect(positionX, positionY, image.width(), image.height());

            painter.drawImage(cardRect, image);
        }


    // Paints the text box if it is enabled
    if (textBoxEnabled) {
        drawTextBox(textBoxString);
    }


    painter.end();
}

float elapsedTime = 0.0f;

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

    // Stop player bodies if they reach the target position
    for (b2Body* playerDiscardBody : playerDiscardBodies) {
        playerDiscardBody->SetLinearVelocity(b2Vec2(-6.0f, 0.0f)); // Velocity direction left
        if (playerDiscardBody->GetPosition().x <= playerDiscardBound) {
            playerDiscardBody->SetLinearVelocity(b2Vec2_zero);
        }
    }

    // Stop player bodies if they reach the target position
    for (b2Body* dealerDiscardBody : dealerDiscardBodies) {
        dealerDiscardBody->SetLinearVelocity(b2Vec2(14.0f, 0.0f)); // Velocity direction left
        if (dealerDiscardBody->GetPosition().x >= dealerDiscardBound) {
            dealerDiscardBody->SetLinearVelocity(b2Vec2_zero);
        }
    }

    elapsedTime += 1.0f / 60.0f;

    // Oscillation parameters
    float tiltAmplitude = M_PI / 20; // Adjust the tilt amplitude as needed
    float tiltFrequency = 3.0f; // Adjust the frequency of oscillation as needed

    // Calculate the tilt angle using a sine wave
    float tiltAngle = sin(elapsedTime * tiltFrequency) * tiltAmplitude;

    // Apply this tilt angle to each card body
    for (b2Body* body : dealerBodies) {
        applyTiltAngle(body, tiltAngle);
    }
    for (b2Body* body : playerBodies) {
        applyTiltAngle(body, tiltAngle);
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
    doubleDown = false;
    textBoxEnabled = false;
    textBoxString = "";

    // Removes all dealer bodies from the world
    for (b2Body* dealerBody : dealerBodies) {
        //world.DestroyBody(dealerBody);
        dealerDiscardBodies.append(dealerBody);
    }
    // Removes all player bodies from the world
    for (b2Body* playerBody : playerBodies) {
        //world.DestroyBody(playerBody);
        playerDiscardBodies.append(playerBody);
    }

    for (QImage& dealerImage : dealerImages) {
        dealerDiscardImages.append(dealerImage);
    }

    for (QImage& playerImage : playerImages) {
        playerDiscardImages.append(playerImage);
    }

    qDebug() << dealerDiscardBodies.size();

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
        image = image.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    }

    for (QImage &image : playerImages) {
        image = image.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);
    }

    dealerDiscardBound = (this->width() / 80.0) - 1.0;

    // Repaint the widget with the new sizes
    update();
}

void SceneWidget::drawTextBox(const QString &text) {
    QPainter painter(this);

    // Set rectangle options
    QRectF textBoxRect(this->width() * 0.76, this->height() * 0.001, this->width() * 0.2, this->height());
    painter.setBrush(QBrush(QColor::fromRgb(46, 77, 62)));
    painter.setPen(QColor::fromRgb(46, 77, 62));
    painter.drawRoundedRect(textBoxRect, 10, 10);

    // Set text options
    QFont font;
    font.setPointSize(this->height() * 0.03);
    painter.setFont(font);
    painter.setPen(Qt::white);
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WordWrap);

    // Draw within center
    painter.drawText(textBoxRect, Qt::AlignCenter, text);
}

void SceneWidget::enableTextBox() {
    textBoxEnabled = true;
}

void SceneWidget::setTextBox(const QString &text) {
    textBoxString = text;
}

void SceneWidget::setShakingEnabled(bool enabled) {
    isShakingEnabled = enabled;
}

void SceneWidget::applyTiltAngle(b2Body* body, float angle) {
    b2Vec2 position = body->GetPosition();
    body->SetTransform(position, angle); // Directly sets the angle of the body
}

void SceneWidget::clearDiscardPile() {
    // Removes all dealer bodies from the world
//    for (b2Body* dealerDiscardBody : dealerDiscardBodies) {
//        world.DestroyBody(dealerDiscardBody);
//    }

    QTimer::singleShot(500, this, [this]() {
        playerDiscardBound *= -2.0;
        dealerDiscardBound *= 2.0;
    });

    QTimer::singleShot(1000, this, [this]() {

        for (b2Body* playerBody : playerDiscardBodies) {
            world.DestroyBody(playerBody);
        }
        for (b2Body* dealerBody : dealerDiscardBodies) {
            world.DestroyBody(dealerBody);
        }

//        playerBodies.clear();
//        playerImages.clear();
        playerDiscardBodies.clear();
        playerDiscardImages.clear();
//        dealerBodies.clear();
//        dealerImages.clear();
        dealerDiscardBodies.clear();
        dealerDiscardImages.clear();

        playerDiscardBound = 0.75;
        dealerDiscardBound = (this->width() / 80.0) - 1.0;
    });

}

void SceneWidget::setPlayerOrDealer(bool isPlayer) {
    isPlayerHand = isPlayer;
}

b2Body* SceneWidget::createCoinBody(float x, float y) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y); // Starting position for the coin

    b2Body* body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f; // Adjust the size as needed

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f; // A bit of a bounce

    body->CreateFixture(&fixtureDef);

    return body;
}

// Method to add a coin to the world
void SceneWidget::addCoin(float x) {
    float startX = x; // Assume this is already in the correct coordinate system for your screen
    float startY = -50.0f; // Start above the screen, adjust as necessary

    b2Body* coinBody = createCoinBody(startX, startY);
    coinBodies.append(coinBody);

    // Apply initial velocity to simulate falling straight down
    float initialVerticalVelocity = 300.0f; // Adjust this speed as necessary
    coinBody->SetLinearVelocity(b2Vec2(0.0f, initialVerticalVelocity));

    // Load and scale the coin image
    QImage coinImage(":/cards/chip.png");
    // Adjust size as necessary
    coinImage = coinImage.scaled(50, 50, Qt::KeepAspectRatio);
    coinImages.append(coinImage);
}

void SceneWidget::spawnCoin() {
    if (isPlayerHand) {
        gen = std::mt19937(rd()); // Seed the generator
        dis = std::uniform_real_distribution<>(this->width() / 4, this->width() / 1.4);
        float randomX = dis(gen);

        addCoin(randomX);
    }
}
