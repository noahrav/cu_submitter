#include "transfer.h"

namespace transfer {

    std::shared_ptr<data::Changelog> DevbuildTransferer::transferChangelog_;
    std::string DevbuildTransferer::base_path_;
    std::string DevbuildTransferer::origin_path_;
    std::string DevbuildTransferer::destination_path_;

    std::shared_ptr<data::Changelog> DevbuildTransferer::getTransferChangelog(const std::string& base_path, const std::string &modified_path) {
        if (base_path.empty()) {
            error("Base devbuild path not defined");
            return nullptr;
        }
        if (modified_path.empty()) {
            error("Origin path not defined");
            return nullptr;
        }

        base_path_ = base_path;
        origin_path_ = modified_path;

        log("Scanning differences...");

        transferChangelog_ = chgen::ChangelogGenerator::scan(base_path_, origin_path_);
        
        return transferChangelog_;
    }

    std::shared_ptr<data::Changelog> DevbuildTransferer::getTransferChangelog() {
        return transferChangelog_;
    }

    void DevbuildTransferer::transferAssets(const data::AssetCategory& category) {
        std::string folder;
        std::vector<data::Asset> assets;
        switch (category) {
            case data::AssetCategory::MENU_THEME:
                folder = "System";
                assets = transferChangelog_->menu_themes_;
                break;
            case data::AssetCategory::CHARSET:
                folder = "CharSet";
                assets = transferChangelog_->charsets_;
                break;
            case data::AssetCategory::CHIPSET:
                folder = "ChipSet";
                assets = transferChangelog_->chipsets_;
                break;
            case data::AssetCategory::MUSIC:
                folder = "Music";
                assets = transferChangelog_->musics_;
                break;
            case data::AssetCategory::SOUND:
                folder = "Sound";
                assets = transferChangelog_->sounds_;
                break;
            case data::AssetCategory::PANORAMA:
                folder = "Panorama";
                assets = transferChangelog_->panoramas_;
                break;
            case data::AssetCategory::PICTURE:
                folder = "Picture";
                assets = transferChangelog_->pictures_;
                break;
            case data::AssetCategory::BATTLE_ANIMATION:
                folder = "Battle";
                assets = transferChangelog_->animation_files_;
                break;
        }

        const auto origin_asset_folder = std::string(origin_path_ / fs::path(folder));
        const auto destination_asset_folder = std::string(destination_path_ / fs::path(folder));

        if (!fs::exists(destination_asset_folder)) {
            error("Missing file: " + destination_asset_folder);
            return;
        }

        for (const auto& asset: assets) {
            const auto origin_asset = origin_asset_folder / fs::path(asset.filename_);
            const auto destination_asset = destination_asset_folder / fs::path(asset.filename_);

            switch(asset.status_) {
            case data::Status::REMOVED:
                log("Removing " + std::string(destination_asset));

                fs::remove(destination_asset);
                break;
            case data::Status::MODIFIED:
                log("Updating " + std::string(destination_asset));

                fs::copy(origin_asset, destination_asset, fs::copy_options::update_existing);
                break;
            case data::Status::ADDED:
                log("Adding " + std::string(destination_asset));

                fs::copy(origin_asset, destination_asset, fs::copy_options::update_existing);
                break;
            }
        }
    }

    void DevbuildTransferer::transferMaps(void) {
        for (const auto& ce: transferChangelog_->maps_) {
            const auto origin_map = origin_path_ / fs::path("Map" + data::id_string(ce.id_) + ".lmu");
            const auto destination_map = destination_path_ / fs::path("Map" + data::id_string(ce.id_) + ".lmu");

            switch(ce.status_) {
            case data::Status::REMOVED:
                log("Removing " + std::string(destination_map));

                //TODO: Reset to blank ce
                break;
            case data::Status::MODIFIED:
                log("Updating " + std::string(destination_map));

                fs::copy(origin_map, destination_map, fs::copy_options::update_existing);
                break;
            case data::Status::ADDED:
                log("Adding " + std::string(destination_map));

                fs::copy(origin_map, destination_map, fs::copy_options::update_existing);
                break;
            }
        }
    }

    void DevbuildTransferer::transferCE(void) {
        for (const auto& ce: transferChangelog_->common_events_) {
            const auto origin_ce = origin_db_->commonevents[ce.id_];

            if (origin_ce.ID != ce.id_) {
                error("IDs are not properly ordered");
                continue;
            }

            switch(ce.status_) {
            case data::Status::REMOVED:
                log("Removing Common Event " + data::id_string(ce.id_));

                //TODO: Reset to blank ce
                break;
            case data::Status::MODIFIED:
                log("Updating Common Event " + data::id_string(ce.id_));

                destination_db_->commonevents[ce.id_] = origin_ce;
                break;
            case data::Status::ADDED:
                log("Adding Common Event " + data::id_string(ce.id_));

                destination_db_->commonevents[ce.id_] = origin_ce;
                break;
            }
        }
    }

    void DevbuildTransferer::transferTilesets(void) {
        for (const auto& tileset: transferChangelog_->tilesets_) {
            const auto origin_tileset = origin_db_->chipsets[tileset.id_];

            if (origin_tileset.ID != tileset.id_) {
                error("IDs are not properly ordered");
                continue;
            }

            switch(tileset.status_) {
            case data::Status::REMOVED:
                log("Removing Tileset " + data::id_string(tileset.id_));

                //TODO: Reset to blank tileset entry
                break;
            case data::Status::MODIFIED:
                log("Updating Tileset " + data::id_string(tileset.id_));

                destination_db_->chipsets[tileset.id_] = origin_tileset;
                break;
            case data::Status::ADDED:
                log("Adding Tileset " + data::id_string(tileset.id_));

                destination_db_->chipsets[tileset.id_] = origin_tileset;
                break;
            }
        }
    }

    void DevbuildTransferer::transferSwitches(void) {
        for (const auto& switch_: transferChangelog_->switches_) {
            const auto origin_switch = origin_db_->switches[switch_.id_];

            if (origin_switch.ID != switch_.id_) {
                error("IDs are not properly ordered");
                continue;
            }

            switch(switch_.status_) {
            case data::Status::REMOVED:
                log("Removing Switch " + data::id_string(switch_.id_));

                //TODO: Reset to blank switch
                break;
            case data::Status::MODIFIED:
                log("Updating Switch " + data::id_string(switch_.id_));

                destination_db_->switches[switch_.id_] = origin_switch;
                break;
            case data::Status::ADDED:
                log("Adding Switch " + data::id_string(switch_.id_));

                destination_db_->switches[switch_.id_] = origin_switch;
                break;
            }
        }
    }

    void DevbuildTransferer::transferVariables(void) {
        for (const auto& variable: transferChangelog_->variables_) {
            const auto origin_variable = origin_db_->variables[variable.id_];

            if (origin_variable.ID != variable.id_) {
                error("IDs are not properly ordered");
                continue;
            }

            switch(variable.status_) {
            case data::Status::REMOVED:
                log("Removing Variable " + data::id_string(variable.id_));

                //TODO: Reset to blank variable
                break;
            case data::Status::MODIFIED:
                log("Updating Variable " + data::id_string(variable.id_));

                destination_db_->variables[variable.id_] = origin_variable;
                break;
            case data::Status::ADDED:
                log("Adding Variable " + data::id_string(variable.id_));

                destination_db_->variables[variable.id_] = origin_variable;
                break;
            }
        }
    }

    void DevbuildTransferer::transferAnimations(void) {
        for (const auto& animation: transferChangelog_->animations_) {
            const auto origin_animation = origin_db_->animations[animation.id_];

            if (origin_animation.ID != animation.id_) {
                error("IDs are not properly ordered");
                continue;
            }

            switch(animation.status_) {
            case data::Status::REMOVED:
                log("Removing Animation " + data::id_string(animation.id_));

                //TODO: Reset to blank animation entry
                break;
            case data::Status::MODIFIED:
                log("Updating Animation " + data::id_string(animation.id_));

                destination_db_->animations[animation.id_] = origin_animation;
                break;
            case data::Status::ADDED:
                log("Adding Animation " + data::id_string(animation.id_));

                destination_db_->animations[animation.id_] = origin_animation;
                break;
            }
        }
    }

    void DevbuildTransferer::transfer(const std::string& to)
    {
        if (base_path_.empty()) {
            error("Base devbuild path not defined");
            return;
        }
        if (origin_path_.empty()) {
            error("Origin path not defined");
            return;
        }
        if (to.empty()) {
            error("Destination path not defined");
            return;
        }

        if (!transferChangelog_) {
            error("Transfer changelog not defined. You need to scan your modifications before calling getTransferChangelog.");
            return;
        }

        destination_path_ = to;

        transferAssets(data::AssetCategory::MENU_THEME);
        transferAssets(data::AssetCategory::CHARSET);
        transferAssets(data::AssetCategory::CHIPSET);
        transferAssets(data::AssetCategory::MUSIC);
        transferAssets(data::AssetCategory::SOUND);
        transferAssets(data::AssetCategory::PANORAMA);
        transferAssets(data::AssetCategory::PICTURE);
        transferAssets(data::AssetCategory::BATTLE_ANIMATION);

        transferMaps();

        origin_db_ = lcf::LDB_Reader::Load(std::string(origin_path_ / fs::path("RPG_RT.ldb")));
        destination_db_ = lcf::LDB_Reader::Load(std::string(destination_path_ / fs::path("RPG_RT.ldb")));

        if (!origin_db_) {
            error("Could not read " + std::string(origin_path_ / fs::path("RPG_RT.ldb")));
            return;
        }
        if (!destination_db_) {
            error("Could not read " + std::string(destination_path_ / fs::path("RPG_RT.ldb")));
            return;
        }

        transferCE();
        transferTilesets();
        transferSwitches();
        transferVariables();
        transferAnimations();
    }

    void DevbuildTransferer::exportChangelog() {
        if (destination_path_.empty()) {
            error("Destination path not defined");
            return;
        }
        if (!transferChangelog_) {
            error("No changelog scanned");
            return;
        }

        fs::path export_path = destination_path_;

        chgen::ChangelogGenerator::generate(transferChangelog_, export_path.parent_path());
    }

} // transfer