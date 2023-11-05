#include <iostream>

#include <QApplication>
#include <QLabel>

#include "chgen/chgen.h"
#include "transfer/transfer.h"
#include "utils/error.h"
#include "utils/log.h"
#include "utils/print.h"

/**
 * @program cu_submitter
 * @brief The main entry point for the program.
 */

int main(int argc, char* argv[])
{
    if (argc >= 2) {
        //CLI mode
        const std::string option = argv[1];

        if (option == "--help" || option == "--usage") {
            std::string usage_message = "USAGE\n";
            usage_message += "-----\n";
            usage_message += "--help | --usage : prints this message\n";
            usage_message += "--chgen <base_path> <modified_path> : generates a changelog text file\n";
            usage_message += "--transfer <unmodified_copy_path> <modified_copy_path> <destination_path>";

            print(usage_message);
        } else if (option == "--chgen") {
            if (argc < 4) {
                error("Not enough arguments");
                return 1;
            }

            const auto changelog = chgen::ChangelogGenerator::scan(argv[2], argv[3]);
            if (changelog == nullptr) {
                error("Could not generate changelog");
                return 1;
            }

            chgen::ChangelogGenerator::generate(changelog);
        } else if (option == "--transfer") {
            if (argc < 5) {
                error("Not enough arguments");
                return 1;
            }

            const std::string base = argv[2];
            const std::string from = argv[3];
            const std::string to = argv[4];

            const auto changelog = transfer::DevbuildTransferer::getTransferChangelog(base, from);

            if (changelog == nullptr) {
                error("Could not generate changelog");
                return 1;
            }

            std::cout << "Changelog: \n";
            std::cout << changelog->stringify() << "\n\n";

            std::cout << "Confirm transfer ? (O/N) ";
            char confirm;
            std::cin >> confirm;
            if (confirm == 'N' || confirm == 'n') {
                error("Transfer cancelled");
                return 1;
            }

            transfer::DevbuildTransferer::transfer(to);

            transfer::DevbuildTransferer::exportChangelog();
        } else {
            error("Invalid arguments");
            return 1;
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
