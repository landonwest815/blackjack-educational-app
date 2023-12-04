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
    // Set the custom font for the widget
    QFont textFont("Arial", 18, QFont::Bold, true); // true for italic
    setFont(textFont);
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

    // Set up the font according to the stylesheet
    QFont textFont("Arial", 18, QFont::Bold, true); // true for italic
    painter.setFont(textFont);

    // Draw the background bubble
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::white);

    QRect bubbleRect = rect();
    int radius = 10;
    painter.drawRoundedRect(bubbleRect, radius, radius);

    // Set up the pen for the text
    QPen textPen(Qt::black); // Black color for the text
    painter.setPen(textPen);
    QString displayText = QString(adviceText);
    painter.drawText(bubbleRect.adjusted(10, 10, -10, -10), Qt::AlignCenter | Qt::TextWordWrap, displayText);
}


void HelpWidget::setSize() {
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent());
    if (mainWindow) {
        QFontMetrics metrics(font());
        int wrapWidth = mainWindow->width() / 4.0 - 30.0;
        QRect textRect = metrics.boundingRect(0, 0, wrapWidth, INT_MAX, Qt::TextWordWrap | Qt::AlignLeft, adviceText);
        setFixedSize(textRect.width() + 2 * 10, textRect.height() + 2 * 17.5);
    }
    update();
    reposition();
}

void HelpWidget::reposition() {
    if (QMainWindow *mainWindow = qobject_cast<QMainWindow*>(parent())) {
        int marginRight = mainWindow->width() / 3.75;
        int verticalCenter = (mainWindow->height() - height()) / 2.0 + 150.0;
        int newX = mainWindow->width() - width() - marginRight;
        int newY = verticalCenter;
        move(newX, newY);
    }
}
