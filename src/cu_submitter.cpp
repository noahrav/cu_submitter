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
    if (argc >= 2) {
        //CLI mode

        if (std::string(argv[1]) == "--chgen") {
            if (argc < 4) {
                std::cerr << "Error: Not enough arguments." << '\n';
                return 1;
            }

            auto changelog = chgen::ChangelogGenerator::scan(argv[2], argv[3]);
            if (changelog == nullptr) {
                std::cerr << "Error: Could not generate changelog." << '\n';
                return 1;
            }

            chgen::ChangelogGenerator::generate(changelog);
        }

        return 0;
    }

    QApplication MainWindow(argc, argv);
    QLabel MainLabel("<center>Collective Unconscious Submitter</center>");
    MainLabel.setWindowTitle("CU Submitter");
    MainLabel.resize(400, 400);
    MainLabel.show();
    return MainWindow.exec();
}
