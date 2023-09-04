#include <iostream>

#include <QApplication>
#include <QLabel>

#include "chgen/chgen.h"

/**
 * @program cu_submitter
 * @brief The main entry point for the program.
 */

int main(int argc, char* argv[])
{
    QApplication MainWindow(argc, argv);
    QLabel MainLabel("<center>Collective Unconscious Changelog Generator</center>");
    MainLabel.setWindowTitle("CU Changelog Generator");
    MainLabel.resize(400, 400);
    MainLabel.show();
    return MainWindow.exec();
}
