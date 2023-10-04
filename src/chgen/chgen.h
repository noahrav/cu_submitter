#ifndef CU_SUBMITTER_CHGEN_H
#define CU_SUBMITTER_CHGEN_H

#include <fstream>
#include <iostream>
#include <filesystem>
#include <lcf/lmt/reader.h>
#include <lcf/lmu/reader.h>
#include <lcf/ldb/reader.h>
#include "../data/changelog.h"

namespace fs = std::filesystem;

namespace chgen {

    class ChangelogGenerator {
    public:
        /**
         * @brief Scans the base and modified paths for changes.
         * @param base_path
         * @param modified_path
         * @return A changelog object.
         */
        static std::shared_ptr<data::Changelog> scan(const std::string& base_path, const std::string& modified_path);

        /**
         * @brief Generates a changelog file.
         * @param changelog
         */
        static void generate(const std::shared_ptr<data::Changelog>& changelog);
    };

} // chgen

#endif //CU_SUBMITTER_CHGEN_H
