#include "chgen.h"

#include <utility>

namespace chgen {

    std::vector<std::string> list_directory_content(std::string path) {
        std::vector<std::string> content;

        try {
            if (!fs::exists(path)) {
                std::cerr << "The specified path does not exist." << '\n';
                return content;
            }

            if (!fs::is_directory(path)) {
                std::cerr << "The specified path is not a directory." << '\n';
                return content;
            }

            for (const auto &file: fs::directory_iterator(path)) {
                content.push_back(file.path().filename().string());
            }

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << '\n';
        }

        return content;
    }

    std::vector<data::BGMEvent> list_bgm_events(std::unique_ptr<lcf::rpg::Map> map) {
        std::vector<data::BGMEvent> bgm_events;

        for (const auto &event: map->events) {
            for (const auto &page: event.pages) {
                for (const auto &command: page.event_commands) {
                    if (command.code == 11510) {
                        data::BGMEvent bgm_event;
                        bgm_event.status_ = data::Status::ADDED;
                        bgm_event.coordinates_.x = event.x;
                        bgm_event.coordinates_.y = event.y;
                        bgm_event.track_name_ = command.string.data();
                        auto params = command.parameters.begin();
                        bgm_event.volume_ = *std::next(params, 1);
                        bgm_event.speed_ = *std::next(params, 2);
                        bgm_events.push_back(bgm_event);
                    }
                }
            }
        }

        return bgm_events;
    }

    std::shared_ptr<data::Changelog>
    ChangelogGenerator::scan(const std::string &base_path, const std::string &modified_path) {
        auto base_content = list_directory_content(base_path);
        auto modified_content = list_directory_content(modified_path);

        auto changelog = std::make_shared<data::Changelog>();

        // TODO: get developer name
        changelog->developer_ = "No dev name";

        auto now = time(nullptr);
        changelog->date_ = localtime(&now);

        // TODO: get summary name
        changelog->summary_ = "";

        // TODO: get policies
        changelog->map_policy_ = "";
        changelog->asset_policy_ = "";

        // maps
        std::vector<std::string> base_maps;
        std::remove_copy_if(begin(base_content), end(base_content), std::back_inserter(base_maps),
                            [](const std::string &s) {
                                return s.substr(s.find_last_of('.') + 1) != "lmu";
                            });

        std::vector<std::string> modified_maps;
        std::remove_copy_if(begin(modified_content), end(modified_content), std::back_inserter(modified_maps),
                            [](const std::string &s) {
                                return s.substr(s.find_last_of('.') + 1) != "lmu";
                            });

        // map tree
        fs::path base_lmt_path = base_path / fs::path("RPG_RT.lmt");
        if (!fs::exists(base_lmt_path)) {
            std::cerr << "Missing file: " + std::string(base_lmt_path) << '\n';
            return nullptr;
        }

        auto base_map_tree = lcf::LMT_Reader::Load(std::string(base_lmt_path));

        fs::path modified_lmt_path = modified_path / fs::path("RPG_RT.lmt");
        if (!fs::exists(modified_lmt_path)) {
            std::cerr << "Missing file: " + std::string(modified_lmt_path) << '\n';
            return nullptr;
        }

        auto modified_map_tree = lcf::LMT_Reader::Load(std::string(modified_lmt_path));

        return changelog;
    }

    void ChangelogGenerator::generate(const std::shared_ptr<data::Changelog> &changelog) {
        std::ofstream file;
        // TODO: generate new file name
        file.open("changelog.txt");

        file << changelog->stringify() << '\n';

        file.close();
    }

} // chgen