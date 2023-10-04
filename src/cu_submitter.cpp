#include <iostream>

#include <QApplication>
#include <QLabel>

#include "chgen/chgen.h"
#include "utils/error.h"

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
                error("Error: Not enough arguments.");
                return 1;
            }

            auto changelog = chgen::ChangelogGenerator::scan(argv[2], argv[3]);
            if (changelog == nullptr) {
                error("Error: Could not generate changelog.");
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
