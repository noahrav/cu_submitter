#ifndef CU_SUBMITTER_TRANSFER_H
#define CU_SUBMITTER_TRANSFER_H

#include <filesystem>

#include "../chgen/chgen.h"
#include "../utils/error.h"
#include "../utils/log.h"

namespace fs = std::filesystem;

namespace transfer {

    class DevbuildTransferer {
    public:
        /**
         * Scans changes via the changelog generator to list modifications. Needs to be called before a transfer
         * @param base_path Unmodified copy of the devbuild from the version you were working on
         * @param modified_path The copy you modified
         * @returns Your modifications in the format of a changelog
         */
        static std::shared_ptr<data::Changelog> getTransferChangelog(const std::string& base_path_, const std::string &origin_path);
        /**
         * Returns the last scanned transfer changelog
         * @returns The last scanned transfer's changelog
         */
        static std::shared_ptr<data::Changelog> getTransferChangelog();

        /**
         * Moves or override your changes into the newest devbuild. getTransferChangelog must be called beforehand
         * @param to Unmodified copy of the newest devbuild
         */
        static void transfer(const std::string& to);

        /**
         * Exports the last scanned transfer changelog to a text file inside destination_path_
         */
        static void exportChangelog();

    private:
        /**
         * Transfers assets
         * @param assets Vector of asset changelog objects
         * @param category The category of the assets. Must correspond with the category of the assets in the assets vector
         */
        static void transferAssets(const std::vector<data::Asset>& assets, const data::AssetCategory& category);
        /**
         * Transfer map files
         * @param maps Vector of map changelog objects
         */
        static void transferMaps(const std::vector<data::Map>& maps);

        static std::shared_ptr<data::Changelog> transferChangelog_;

        static std::string base_path_;
        static std::string origin_path_;
        static std::string destination_path_;
    };

} // transfer

#endif //CU_SUBMITTER_TRANSFER_H
