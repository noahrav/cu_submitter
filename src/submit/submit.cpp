#include "submit.h"

namespace submit {

    static std::shared_ptr<data::Changelog> submissionChangelog_;

    static std::string base_path_;
    static std::string modified_path_;

    std::shared_ptr<data::Changelog> getSubmissionChangelog(const std::string& base_path, const std::string &modified_path) {
        if (base_path.empty()) {
            error("Base devbuild path not defined");
            return nullptr;
        }
        if (modified_path.empty()) {
            error("Origin path not defined");
            return nullptr;
        }

        base_path_ = base_path;
        modified_path_ = modified_path;

        log("Scanning differences...");

        submissionChangelog_ = chgen::ChangelogGenerator::scan(base_path_, modified_path_);
        
        return submissionChangelog_;
    }

    std::shared_ptr<data::Changelog> SubmissionBuilder::getSubmissionChangelog() {
        return submissionChangelog_;
    }

    void SubmissionBuilder::submitAssets(const data::AssetCategory& category) {
        std::string folder;
        std::vector<data::Asset> assets;
        switch (category) {
            case data::AssetCategory::MENU_THEME:
                folder = "System";
                assets = submissionChangelog_->menu_themes_;
                break;
            case data::AssetCategory::CHARSET:
                folder = "CharSet";
                assets = submissionChangelog_->charsets_;
                break;
            case data::AssetCategory::CHIPSET:
                folder = "ChipSet";
                assets = submissionChangelog_->chipsets_;
                break;
            case data::AssetCategory::MUSIC:
                folder = "Music";
                assets = submissionChangelog_->musics_;
                break;
            case data::AssetCategory::SOUND:
                folder = "Sound";
                assets = submissionChangelog_->sounds_;
                break;
            case data::AssetCategory::PANORAMA:
                folder = "Panorama";
                assets = submissionChangelog_->panoramas_;
                break;
            case data::AssetCategory::PICTURE:
                folder = "Picture";
                assets = submissionChangelog_->pictures_;
                break;
            case data::AssetCategory::BATTLE_ANIMATION:
                folder = "Battle";
                assets = submissionChangelog_->animation_files_;
                break;
        }

        const auto base_asset_folder = std::string(modified_path_ / fs::path(folder));
        const auto destination_asset_folder = std::string(archive_path_ / fs::path(folder));

        if (!fs::create_directory(destination_asset_folder)) {
            error("Could not create: " + destination_asset_folder);
            return;
        }

        for (const auto& asset: assets) {
            const auto origin_asset = base_asset_folder / fs::path(asset.filename_);

            switch(asset.status_) {
            case data::Status::REMOVED:
                break;
            case data::Status::MODIFIED:
                log("Moving " + std::string(origin_asset) + " into " + destination_asset_folder);

                fs::copy(origin_asset, destination_asset_folder);
                break;
            case data::Status::ADDED:
                log("Moving " + std::string(origin_asset) + " into " + destination_asset_folder);

                fs::copy(origin_asset, destination_asset_folder);
                break;
            }
        }
    }

    void SubmissionBuilder::submitMaps() {
        auto blank_map = lcf::rpg::Map();

        for (const auto& map: submissionChangelog_->maps_) {
            const auto origin_map = base_path_ / fs::path("Map" + data::id_string(map.id_) + ".lmu");

            switch(map.status_) {
            case data::Status::REMOVED:
                break;
            case data::Status::MODIFIED:
                log("Moving " + std::string(origin_map) + " into " + archive_path_);

                fs::copy(origin_map, archive_path_);
                break;
            case data::Status::ADDED:
                log("Moving " + std::string(origin_map) + " into " + archive_path_);

                fs::copy(origin_map, archive_path_);
                break;
            }
        }
    }

    void SubmissionBuilder::submit(const std::string& archive_path) {
        if (!submissionChangelog_) {
            error("Submission changelog not defined. You need to scan your modifications before calling submit.");
            return;
        }

        try {
            fs::create_directories(archive_path);

            archive_path_ = archive_path;

            fs::copy(modified_path_ / fs::path("RPG_RT.lbd"), archive_path);
            fs::copy(modified_path_ / fs::path("RPG_RT.lmt"), archive_path);

            submitMaps();

            submitAssets(data::AssetCategory::MENU_THEME);
            submitAssets(data::AssetCategory::CHARSET);
            submitAssets(data::AssetCategory::CHIPSET);
            submitAssets(data::AssetCategory::MUSIC);
            submitAssets(data::AssetCategory::SOUND);
            submitAssets(data::AssetCategory::PANORAMA);
            submitAssets(data::AssetCategory::PICTURE);
            submitAssets(data::AssetCategory::BATTLE_ANIMATION);

        } catch (const std::exception &e) {
            error(std::string(e.what()));
        }
    }

    void SubmissionBuilder::submit() {
        if (!submissionChangelog_) {
            error("Submission changelog not defined. You need to scan your modifications before calling submit.");
            return;
        }

        const std::string date = data::date_string(submissionChangelog_->date_);
        const std::string date_formatted = date.substr(0, 2) + date.substr(3, 3) + date.substr(7, date.length());

        const std::string dev_name = "NoDevName"; //TODO: replace NoDevName by actual dev name
        const std::string output_name = dev_name + "_submission_" + date_formatted;

        submit(output_name); 
    }
} // submit
