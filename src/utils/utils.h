#ifndef CU_SUBMITTER_UTILS_H
#define CU_SUBMITTER_UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "error.h"

namespace utils {

    /**
     * @brief Compares two files. Returns true if they are the same, false otherwise.
     * @param path1
     * @param path2
     * @return True if the files are the same, false otherwise.
     */
    bool compareFiles(const std::string& path1, const std::string& path2);

} // utils

#endif //CU_SUBMITTER_UTILS_H
