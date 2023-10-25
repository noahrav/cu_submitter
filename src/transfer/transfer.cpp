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

    void DevbuildTransferer::transferAssets(const std::vector<data::Asset>& assets, const data::AssetCategory& category) {
        std::string folder;
        switch (category) {
            case data::AssetCategory::MENU_THEME:
                folder = "System";
                break;
            case data::AssetCategory::CHARSET:
                folder = "CharSet";
                break;
            case data::AssetCategory::CHIPSET:
                folder = "ChipSet";
                break;
            case data::AssetCategory::MUSIC:
                folder = "Music";
                break;
            case data::AssetCategory::SOUND:
                folder = "Sound";
                break;
            case data::AssetCategory::PANORAMA:
                folder = "Panorama";
                break;
            case data::AssetCategory::PICTURE:
                folder = "Picture";
                break;
            case data::AssetCategory::BATTLE_ANIMATION:
                folder = "Battle";
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

    void DevbuildTransferer::transferMaps(const std::vector<data::Map>& maps) {
        for (const auto& map: maps) {
            const auto origin_map = origin_path_ / fs::path("Map" + data::id_string(map.id_) + ".lmu");
            const auto destination_map = destination_path_ / fs::path("Map" + data::id_string(map.id_) + ".lmu");

            switch(map.status_) {
            case data::Status::REMOVED:
                log("Removing " + std::string(destination_map));

                //TODO: Reset to blank map
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

        transferAssets(transferChangelog_->menu_themes_, data::AssetCategory::MENU_THEME);
        transferAssets(transferChangelog_->charsets_, data::AssetCategory::CHARSET);
        transferAssets(transferChangelog_->chipsets_, data::AssetCategory::CHIPSET);
        transferAssets(transferChangelog_->musics_, data::AssetCategory::MUSIC);
        transferAssets(transferChangelog_->sounds_, data::AssetCategory::SOUND);
        transferAssets(transferChangelog_->panoramas_, data::AssetCategory::PANORAMA);
        transferAssets(transferChangelog_->pictures_, data::AssetCategory::PICTURE);
        transferAssets(transferChangelog_->animation_files, data::AssetCategory::BATTLE_ANIMATION);

        transferMaps(transferChangelog_->maps_);
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