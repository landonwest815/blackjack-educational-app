#include "helpwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <QPen>
#include <QBrush>
#include <QMainWindow>

HelpWidget::HelpWidget(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_TranslucentBackground);
    reposition();
}

void HelpWidget::setText(const QString &text) {
    adviceText = text;

    setSize();

    update();
}

void HelpWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::white);

    // Draw the rounded rectangle
    QRect bubbleRect = rect();
    int radius = 10; // Adjust the radius for rounder corners
    painter.drawRoundedRect(bubbleRect, radius, radius);

    QString displayText = QString("ℹ️ %1").arg(adviceText);

    // Draw the text inside the bubble
    painter.drawText(bubbleRect.adjusted(10, 10, -10, -10), Qt::AlignCenter | Qt::TextWordWrap, displayText);
}

void HelpWidget::setSize() {
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent());
    if (mainWindow) {
        QFontMetrics metrics(font());
        int wrapWidth = mainWindow->width() / 4.0 - 30.0;
        QRect textRect = metrics.boundingRect(0, 0, wrapWidth, INT_MAX, Qt::TextWordWrap | Qt::AlignLeft, adviceText);
        setFixedSize(textRect.width() + 2 * 10, textRect.height() + 2 * 10);
    }
    update();
    reposition();
}

void HelpWidget::reposition() {
    if (QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent())) {
        int marginRight = mainWindow->width() / 4.25; // Margin from the right edge of the parent
        int verticalCenter = (mainWindow->height() - height()) / 1.66; // Calculate vertical center

        int newX = mainWindow->width() - width() - marginRight;
        int newY = verticalCenter;

        move(newX, newY); // Reposition the widget
    }
}
