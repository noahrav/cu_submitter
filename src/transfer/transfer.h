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
         * @param category The category of the assets. Must correspond with the category of the assets in the assets vector
         */
        static void transferAssets(const data::AssetCategory& category);
        /**
         * Transfer map files
         */
        static void transferMaps();
        /**
         * Transfer common events
         */
        static void transferCE();
        /**
         * Transfer tileset entries
         */
        static void transferTilesets();
        /**
         * Transfer switches
         */
        static void transferSwitches();
        /**
         * Transfer variables
         */
        static void transferVariables();
        /**
         * Transfer animation entries
         */
        static void transferAnimations();
        /**
         * Transfer map tree data
        */
        static void transferMapTree();

        static std::shared_ptr<data::Changelog> transferChangelog_;

        static std::unique_ptr<lcf::rpg::Database> origin_db_;
        static std::unique_ptr<lcf::rpg::Database> destination_db_;

        static std::unique_ptr<lcf::rpg::TreeMap> origin_maptree_;
        static std::unique_ptr<lcf::rpg::TreeMap> destination_maptree_;

        static std::string base_path_;
        static std::string origin_path_;
        static std::string destination_path_;
    };

} // transfer

#endif //CU_SUBMITTER_TRANSFER_H
