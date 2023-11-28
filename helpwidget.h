#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

class HelpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWidget(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setSize();
    void reposition();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString adviceText;
};

#endif // HELPWIDGET_H
