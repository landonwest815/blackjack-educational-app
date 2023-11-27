#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_swapButtons_clicked();
    void add50();
    void add100();
    void add250();
    void add500();

private:
    Ui::MainWindow *ui;
    bool buttonState;
    int betTotal;
    int bankTotal;


    // Probably will remove, serves as demo functions
    void addDealer();
    void addPlayer();
    void clearAll();





};
#endif // MAINWINDOW_H
