#include "log.h"

void log(const std::string& message) {
    auto now = time(nullptr);
    const auto date = localtime(&now);

    std::string date_string = "[" + std::to_string(date->tm_mday) + "/" + std::to_string(date->tm_mon + 1) + "/" + std::to_string(date->tm_year + 1900);
    date_string += " - " + std::to_string(date->tm_hour) + ":" + std::to_string(date->tm_min) + ":" + std::to_string(date->tm_sec) + "] ";

    std::cout << date_string + message << '\n';
}
