/***
 * Authors:         Team Six of Hearts
 * Members:         Ryan Nguyen, John Nguyen, Christian Hartman, Caleb Funk,
 *                  Landon West, and Pablo Arancibia-Bazan
 * Course:          CS 3505, University of Utah, School of Computing
 * Assignment:      A9 - Educational App
 *
 * Description:     This file creates the main application environment, initializes
 *                  the MainWindow, and displays it in full-screen mode.
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;

    // Display the main window in full-screen mode
    mainWindow.showFullScreen();

    // Start the main event loop of the application
    return a.exec();
}
