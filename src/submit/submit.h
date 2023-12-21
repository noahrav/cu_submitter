#ifndef CU_SUBMITTER_SUBMIT_H
#define CU_SUBMITTER_SUBMIT_H

#include <filesystem>

#include "../chgen/chgen.h"
#include "../utils/error.h"
#include "../utils/log.h"

namespace fs = std::filesystem;

namespace submit {

    class SubmissionBuilder {
    public:
        /**
         * Scans changes via the changelog generator to list modifications. Needs to be called before a submission
         * @param base_path Unmodified copy of the devbuild from the version you were working on
         * @param modified_path The copy you modified
         * @returns Your modifications in the format of a changelog
         */
        static std::shared_ptr<data::Changelog> getSubmissionChangelog(const std::string& base_path, const std::string &modified_path);

        /**
         * Returns the last scanned submission changelog
         * @returns The last scanned submission changelog
         */
        static std::shared_ptr<data::Changelog> getSubmissionChangelog();

        /**
         * Packages your submit into a Zip archive. getSubmissionChangelog must be called beforehand
         * @param archive_path Output file name
         */
        static void submit(const std::string& archive_path);

        /**
         * Packages your submit into a Zip archive with an automatically generated name. getSubmissionChangelog must be called beforehand
         */
        static void submit();

        /**
         * Exports the last scanned transfer changelog to a text file inside archive_path_
         */
        static void exportChangelog();

        /**
         * Compresse archive_path_ into an archive file
        */
        //static void compress();

    private:
        /**
         * Moves assets into the submission folder
         * @param category The category of the assets. Must correspond with the category of the assets in the assets vector
        */
        static void submitAssets(const data::AssetCategory& category);
        /**
         * Moves .lmu map files into the submission folder
        */
        static void submitMaps();
        /**
         * @return the root of the game files
        */
        static fs::path gameRoot();

        static std::shared_ptr<data::Changelog> submissionChangelog_;

        static std::string base_path_;
        static std::string modified_path_;
        static std::string archive_path_;
    };

} // submit

#endif //CU_SUBMITTER_SUBMIT_H
