/***
 * Authors:     Team Six of Hearts
 * Members:     Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *              Landon West, and Pablo Arancibia-Bazan
 * Course:      CS 3505, University of Utah, School of Computing
 * Assignment:  A9 - Educational App
 *
 * Description: Header for the class Help Widget, designed to display advice
 *              or help text within a bubble-like format in a QMainWindow. It
 *              features methods for setting and updating the text, resizing
 *              and repositioning the widget according to the content and parent
 *              window size.
 */

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

///@brief Custom widget to display help and advice text in the application.
class HelpWidget : public QWidget
{
    Q_OBJECT
public:

    ///@brief Constructs a HelpWidget with an optional parent.
    explicit HelpWidget(QWidget *parent = nullptr);

    ///@brief Sets the text to be displayed in the widget.
    void setText(const QString &text);

    ///@brief Adjusts the size of the widget based on its content.
    void setSize();

    ///@brief Repositions the widget within its parent context.
    void reposition();

protected:
    ///@brief Custom paint event to draw the widget's content.
    void paintEvent(QPaintEvent *event) override;

private:
    ///@brief Stores the text that will be displayed in the widget.
    QString adviceText;
};

#endif // HELPWIDGET_H
