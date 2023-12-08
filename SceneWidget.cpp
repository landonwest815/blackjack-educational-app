/***
 * Authors:         Team Six of Hearts
 * Members:         Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *                  Landon West, and Pablo Arancibia-Bazan
 * Course:          CS 3505, University of Utah, School of Computing
 * Assignment:      A9 - Educational App
 *
 * Description:     This file is part of an educational application designed to simulate
 *                  a game of blackjack. It integrates the Box2D physics engine to
 *                  simulate and display the dynamic interactions of cards The main focus
 *                  of this implementation is to create a visually engaging and interactive
 *                  blackjack table.
 */

#include "SceneWidget.h"

SceneWidget::SceneWidget(QWidget *parent)
    : QWidget(parent),
    world(b2Vec2(0.0f, 0.0f)),
    timer(this),
    coinTimer(this),
    playerDiscardBound(0.75),
    elapsedTime(0.0f),
    inTutorial(false),
    split(false),
    nextSplit(false),
    splitCountBool(true),
    splitCount(0),
    doubleDown(false),
    isShakingEnabled(false),
    isPlayerHand(true),
    playerWon(false) {

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

    // Assists in creating random chips to fall down the screen upon player win
    connect(&coinTimer, &QTimer::timeout, this, &SceneWidget::spawnChip);
    coinTimer.start(150);

    // Randomness
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<>(this->width() / 4.0, this->width() / 1.4);

    // Create an image for a face down card
    QImage image(":/cards/facedown.png");
    faceDownImage = image;
}

void SceneWidget::addDealerCard(const QString& imagePath) {
    // Set corresponding bool
    isPlayerHand = false;

    // Creates body of dealer card and adds to the associated vector
    auto dealerBody = createCardBody(2.0, this->height() / 30.0); // Initial position of dealer body
    dealerBodies.push_back(dealerBody);

    // Check if image is loaded successfully
    QImage dealerImage;
    if (!dealerImage.load(imagePath)) {
        qWarning("Failed to load dealer card image");
        return;
    }

    // Creates image of dealer card and adds to the associated vector
    const double scaleFactor = 1.5;
    dealerImage = dealerImage.scaled(this->width() / scaleFactor, this->height() / scaleFactor, Qt::KeepAspectRatio);
    dealerImages.push_back(dealerImage);
}

void SceneWidget::addPlayerCard(const QString& imagePath) {
    // Set corresponding bool
    isPlayerHand = true;

    // Adjust splitting data
    if (splitCountBool) splitCount++;

    // Creates body of player card and adds to the associated vector
    auto playerBody = createCardBody(this->width() / 200.0, this->height() / 200.0); // Initial position of player body
    playerBodies.push_back(playerBody);

    // Load and check image
    QImage playerImage;
    if (!playerImage.load(imagePath)) {
        qWarning("Failed to load player card image");
        return;
    }

    // Creates image of player card and adds to the associated vector
    const double scaleFactor = 1.5;
    playerImage = playerImage.scaled(this->width() / scaleFactor, this->height() / scaleFactor, Qt::KeepAspectRatio);
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

    // Define the shape of the dynamic box
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

    // Setting collision group to ensure correct interaction between bodies
    fixtureDef.filter.groupIndex = -1;
    body->CreateFixture(&fixtureDef);

    return body;
}

void SceneWidget::paintEvent(QPaintEvent *) {

    // Setup Painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // For smoother edges

    // Paint all elements
    drawTokens(painter);
    drawCardArea(painter);
    drawCards(painter);
    drawDiscardPile(painter);
    if (!inTutorial) drawDeck(painter);

    painter.end();
}

void SceneWidget::drawTokens(QPainter &painter) {
    // Draw Tokens
    for (int i = 0; i < chipBodies.size(); ++i) {
        const b2Vec2& position = chipBodies[i]->GetPosition();
        const QImage& coinImage = chipImages[i];
        painter.drawImage(QRectF(position.x, position.y, coinImage.width(), coinImage.height()), coinImage);
    }
}

void SceneWidget::drawCardArea(QPainter &painter) {
    // Determine dimensions
    const int areaWidth = this->width();
    const int areaHeight = this->height();

    // Attributes for border
    const int penWidth = 4;
    const int cornerRadius = 10;

    // Calculate the adjusted positions and dimensions
    int rectWidth = areaWidth / 2.0 - penWidth;
    int rectHeight = areaHeight - penWidth;
    int left = (areaWidth - rectWidth) / 2;
    int top = (areaHeight - rectHeight) / 2;

    // Set the pen and brush
    QPen outlinePen(Qt::white, penWidth, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    painter.setPen(outlinePen);
    painter.setBrush(QBrush(QColorConstants::Transparent));

    // Draw the rounded rectangle
    painter.drawRoundedRect(left, top, rectWidth, rectHeight, cornerRadius, cornerRadius);
}

void SceneWidget::drawCards(QPainter &painter) {
    // Delegate to relative method
    if (!isPlayerHand) drawDealerCards(painter);
    else               drawPlayerCards(painter);
}

void SceneWidget::drawDealerCards(QPainter &painter) {

    // Determine general positioning for all cards
    const float positionMultiplierX = 0.5f * 145.0f;
    const float positionMultiplierY = 0.5f * 10.0f;
    const float angleConversionFactor = 180.0f / M_PI * 0.25f;
    const int cardSpacing = 40;

    for (int i = 0; i < dealerBodies.size(); ++i) {
        // Determine positioning for this card
        const b2Vec2& position = dealerBodies[i]->GetPosition();
        float positionLeftX = position.x * positionMultiplierX;
        float positionY = position.y * positionMultiplierY;

        // Setup sizing
        const QImage& image = dealerImages[i];
        QRect cardRect(positionLeftX - (i * cardSpacing), positionY, image.width(), image.height());

        // Shake card if necessary
        if (isShakingEnabled) {
            painter.save();
            painter.translate(cardRect.center());
            float angle = dealerBodies[i]->GetAngle();
            painter.rotate(angle * angleConversionFactor);
            painter.drawImage(QRect(-image.width() / 2, -image.height() / 2, image.width(), image.height()), image);
            painter.restore();
        } else {
            // Otherwise draw as normally
            painter.drawImage(cardRect, image);
        }
    }
}

void SceneWidget::drawPlayerCards(QPainter &painter) {
    // Constants for positioning and card spacing
    constexpr float positionMultiplierX = 0.5f * 325.0f;
    constexpr float positionMultiplierY = 0.5f * 70.0f;
    constexpr float angleConversionFactor = 180.0f / M_PI * 0.25f;
    constexpr int cardSpacingNormal = 40;
    constexpr int cardSpacingSplit = 30;
    constexpr float positionMultiplierSplitX = 0.5f * 600.0f;

    for (int i = 0; i < playerBodies.size(); ++i) {
        const b2Vec2& position = playerBodies[i]->GetPosition();
        float positionX = position.x * positionMultiplierX;
        float positionY = position.y * positionMultiplierY;

        const QImage& image = playerImages[i];
        QRect cardRect;

        if (split) {
            if (i == 0) {
                cardRect = QRect(positionX, positionY, image.width(), image.height());
            } else if (i == 1) {
                cardRect = QRect(position.x * positionMultiplierSplitX, positionY, image.width(), image.height());
            } else if (i >= 2) {
                if (!nextSplit || i < splitCount) {
                    cardRect = QRect(positionX + (i * cardSpacingSplit) - cardSpacingSplit, positionY, image.width(), image.height());
                } else {
                    cardRect = QRect(position.x * positionMultiplierSplitX + (i * cardSpacingSplit) - ((splitCount - 1) * cardSpacingSplit), positionY, image.width(), image.height());
                }
            }
        } else {
            cardRect = QRect(positionX + (i * cardSpacingNormal), positionY, image.width(), image.height());
        }

        if (isShakingEnabled) {
            painter.save();
            painter.translate(cardRect.center());
            float angle = playerBodies[i]->GetAngle();
            painter.rotate(angle * angleConversionFactor);
            painter.drawImage(QRect(-image.width() / 2, -image.height() / 2, image.width(), image.height()), image);
            painter.restore();
        } else {
            painter.drawImage(cardRect, image);
        }
    }
}


void SceneWidget::drawDiscardPile(QPainter &painter) {
    // Delegate to relative method
    if (isPlayerHand) drawPlayerDiscardPile(painter);
    else              drawDealerDiscardPile(painter);
}

void SceneWidget::drawPlayerDiscardPile(QPainter &painter) {
    // Determine general positioning
    const float positionMultiplierX = 0.5f * 325.0f;
    const float positionMultiplierY = 0.5f * 70.0f;
    const int staggerOffsets[] = {0, -5, -10};

    for (int i = 0; i < playerDiscardBodies.size(); ++i) {
        // Determine positioning for this card
        const b2Vec2& position = playerDiscardBodies[i]->GetPosition();
        float positionLeftX = position.x * positionMultiplierX;
        float positionY = position.y * positionMultiplierY;

        // Draw it with the stagger in mind
        const QImage& image = playerDiscardImages[i];
        QRect cardRect = QRect(positionLeftX - (i < 3 ? staggerOffsets[i] : 0), positionY, image.width(), image.height());
        painter.drawImage(cardRect, image);
    }
}

void SceneWidget::drawDealerDiscardPile(QPainter &painter) {
    // Determine general positioning
    const float positionMultiplierX = 0.5f * 145.0f;
    const float positionMultiplierY = 0.5f * 10.0f;
    const int staggerOffsets[] = {0, -5, -10};

    for (int i = 0; i < dealerDiscardBodies.size(); ++i) {
        // Determine positioning for this card
        const b2Vec2& position = dealerDiscardBodies[i]->GetPosition();
        float positionLeftX = position.x * positionMultiplierX;
        float positionY = position.y * positionMultiplierY;

        // Draw it with the stagger in mind
        const QImage& image = dealerDiscardImages[i];
        QRect cardRect = QRect(positionLeftX - (i < 3 ? staggerOffsets[i] : 0), positionY, image.width(), image.height());
        painter.drawImage(cardRect, image);
    }
}

void SceneWidget::drawDeck(QPainter &painter) {
    // Determine general positioning
    const int numberOfCards = 3;
    const float scaleFactor = 1.5;
    const float positionMultiplierY = 0.5f * 70.0f;
    const int basePositionXPlayer = this->width() / 1.135;
    const int basePositionXDealer = 110;
    const int staggerOffsets[] = {0, -5, -10};
    float positionY = (this->height() / 200.0) * positionMultiplierY;

    // Get the face down card
    QImage image = faceDownImage.scaled(this->width() / scaleFactor, this->height() / scaleFactor, Qt::KeepAspectRatio);

    // Draw the deck
    for (int i = 0; i < numberOfCards; ++i) {
        float positionX = isPlayerHand ? (basePositionXPlayer) : basePositionXDealer;
        positionX -= staggerOffsets[i];
        QRect cardRect = QRect(positionX, positionY, image.width(), image.height());
        painter.drawImage(cardRect, image);
    }
}

void SceneWidget::updateWorld() {
    // Update all cards (drawn and discarded)
    updateCardBodiesVelocity(playerBodies, -6.0f, this->width() / 12.0f);
    updateDiscardedCardBodiesVelocity(playerDiscardBodies, -6.0f, playerDiscardBound);
    updateCardBodiesVelocity(dealerBodies, 14.0f, this->width() / 2.4f);
    updateDiscardedCardBodiesVelocity(dealerDiscardBodies, 14.0f, dealerDiscardBound);

    // Update angles to allow card shaking when needed
    updateTiltAngleForShaking();

    // Update the world
    world.Step(1.0 / 60.0, 6, 2);
    update();
}

void SceneWidget::updateCardBodiesVelocity(QVector<b2Body*>& bodies, float velocityX, float boundaryX) {
    // Move cards left or right until a certain bound
    for (b2Body* body : bodies) {
        body->SetLinearVelocity(b2Vec2(velocityX, 0.0f));
        if ((velocityX < 0 && body->GetPosition().x * 50 <= boundaryX) ||
            (velocityX > 0 && body->GetPosition().x * 50 >= boundaryX)) {
            body->SetLinearVelocity(b2Vec2_zero);
        }
    }
}

void SceneWidget::updateDiscardedCardBodiesVelocity(QVector<b2Body*>& bodies, float velocityX, float boundaryX) {
    // Move cards left or right until a certain bound
    for (b2Body* body : bodies) {
        body->SetLinearVelocity(b2Vec2(velocityX, 0.0f));
        if ((velocityX < 0 && body->GetPosition().x <= boundaryX) ||
            (velocityX > 0 && body->GetPosition().x >= boundaryX)) {
            body->SetLinearVelocity(b2Vec2_zero);
        }
    }
}

void SceneWidget::updateTiltAngleForShaking() {
    // Setup angle measures
    elapsedTime += 1.0f / 60.0f;
    const float tiltAmplitude = M_PI / 20;
    const float tiltFrequency = 3.0f;
    float tiltAngle = sin(elapsedTime * tiltFrequency) * tiltAmplitude;

    // Apply to all cards
    for (b2Body* body : dealerBodies) {
        applyTiltAngle(body, tiltAngle);
    }
    for (b2Body* body : playerBodies) {
        applyTiltAngle(body, tiltAngle);
    }
}

void SceneWidget::clearAllCards() {

    // Reset necessary variables
    splitCount = 0;
    split = false;
    nextSplit = false;
    splitCountBool = true;
    doubleDown = false;

    // Discard all drawn cards
    if (!inTutorial) {
        for (b2Body* dealerBody : dealerBodies) {
            dealerDiscardBodies.append(dealerBody);
        }
        for (b2Body* playerBody : playerBodies) {
            playerDiscardBodies.append(playerBody);
        }

        // Save the discarded cards' images
        for (QImage& dealerImage : dealerImages) {
            dealerDiscardImages.append(dealerImage);
        }
        for (QImage& playerImage : playerImages) {
            playerDiscardImages.append(playerImage);
        }
    }

    // Clear out the relative data
    dealerBodies.clear();
    playerBodies.clear();
    dealerImages.clear();
    playerImages.clear();
}

void SceneWidget::splitPlayerCards() {
    // Allows the player card bodies to be split when only two exist
    if (playerBodies.size() == 2) {
        b2Vec2 newPosition(5.0, this->height() / 200.0);
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
    // Check if dealer has any cards and imagePath is not empty
    if (!dealerBodies.empty() && !imagePath.isEmpty()) {
        QImage dealerImage;

        // Load the image and check for success
        if (!dealerImage.load(imagePath)) {
            qWarning() << "Failed to load dealer card image: " << imagePath;
            return;
        }

        // Scale the image
        dealerImage = dealerImage.scaled(this->width() / 1.5, this->height() / 1.5, Qt::KeepAspectRatio);

        // Update the dealer's hidden card image
        dealerImages[0] = dealerImage;
    }
}

void SceneWidget::doubleDownPlayerCard(const QString& imagePath) {
    // Set doubleDown flag to temporarily modify the behavior of addPlayerCard
    doubleDown = true;

    // Add a sideways player card to represent the double down
    addPlayerCard(imagePath);

    // Reset the doubleDown flag after adding the card
    doubleDown = false;
}

void SceneWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Update the dealer bound for any screen size
    dealerDiscardBound = (this->width() / 80.0) - 1.0;

    // Update randomness bounds
    dis = std::uniform_real_distribution<>(this->width() / 4.0, this->width() / 1.4);


    // Update the world
    update();
}

void SceneWidget::setShakingEnabled(bool enabled) { isShakingEnabled = enabled; }

void SceneWidget::setIsPlayerHand(bool isPlayer) { isPlayerHand = isPlayer; }

void SceneWidget::setIsInTutorial(bool isInTutorial) { inTutorial = isInTutorial; }

void SceneWidget::applyTiltAngle(b2Body* body, float angle) {
    b2Vec2 position = body->GetPosition();
    body->SetTransform(position, angle); // Directly sets the angle of the body
}

void SceneWidget::clearDiscardPile() {
    // Constants for delay times
    const int moveDelay = 500;  // milliseconds
    const int destroyDelay = 1500;  // milliseconds

    // Delay for moving discard piles off screen
    QTimer::singleShot(moveDelay, this, [this]() {
        playerDiscardBound *= -4.0;
        dealerDiscardBound *= 2.0;
    });

    // Delay for destroying discard piles and clearing data
    QTimer::singleShot(destroyDelay, this, [this]() {
        // Destroy player discard bodies
        for (b2Body* playerBody : playerDiscardBodies) {
            world.DestroyBody(playerBody);
        }
        // Destroy dealer discard bodies
        for (b2Body* dealerBody : dealerDiscardBodies) {
            world.DestroyBody(dealerBody);
        }

        // Clear the vectors containing the bodies and images
        playerDiscardBodies.clear();
        playerDiscardImages.clear();
        dealerDiscardBodies.clear();
        dealerDiscardImages.clear();

        // Reset the bounds for the next set of discard piles
        playerDiscardBound = 0.75;
        dealerDiscardBound = (this->width() / 80.0) - 1.0;
    });
}

b2Body* SceneWidget::createChipBody(float x, float y) {

    // Create the physics body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y); // Starting position for the chip
    b2Body* body = world.CreateBody(&bodyDef);

    // Shape
    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f;

    // Other attributes
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;

    body->CreateFixture(&fixtureDef);

    return body;
}

void SceneWidget::addChip(float x) {
    // Create a physics body for the chip
    b2Body* coinBody = createChipBody(x, -50.0f);
    chipBodies.append(coinBody);

    // Apply initial velocity to simulate falling straight down
    coinBody->SetLinearVelocity(b2Vec2(0.0f, 500.0f));

    // Load and scale the coin image
    QImage coinImage(":/cards/chip.png");
    coinImage = coinImage.scaled(50.0f, 50.0f, Qt::KeepAspectRatio);
    chipImages.append(coinImage);
}

void SceneWidget::spawnChip() {
    if (playerWon) {
        // Randomly spawn chips horizontally
        float randomX = dis(gen);
        addChip(randomX);
    }
}

void SceneWidget::setPlayerWon(bool won) {
    playerWon = won;

    if (!won && !chipBodies.empty()) { // Check if the list is not empty
        QTimer::singleShot(3000, this, [this]() {
            // Destroy chip bodies
            for (b2Body* chip : chipBodies) {
                if (chip) {
                    world.DestroyBody(chip);
                }
            }

            // Clear the lists after destruction
            chipBodies.clear();
            chipImages.clear();
        });
    }
}
