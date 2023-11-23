#include "SceneWidget.h"
#include <QPainter>
#include <QDebug>

SceneWidget::SceneWidget(QWidget *parent) : QWidget(parent),
    world(b2Vec2(0.0f, 10.0f)),
    timer(this),
    image(":/cards/2C.png")
{
    backgroundImage.load(":/cards/table.png");

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 20.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9;
    body->CreateFixture(&fixtureDef);
    connect(&timer, &QTimer::timeout, this, &SceneWidget::updateWorld);
    timer.start(10);
}

void SceneWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    backgroundImage = backgroundImage.scaled(900, 900, Qt::KeepAspectRatio);
    painter.drawImage(0, 0, backgroundImage);

    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    image = image.scaled(95, 95, Qt::KeepAspectRatio);
    painter.drawImage((int)(position.x), (int)(position.y), image);
    painter.end();
}

void SceneWidget::updateWorld() {
    world.Step(1.0/60.0, 6, 2);
    update();
}

