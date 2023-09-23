//
// Created by noahr on 09/09/2023.
//

#ifndef CU_SUBMITTER_CHANGELOG_H
#define CU_SUBMITTER_CHANGELOG_H

#include <string>
#include <vector>

namespace data {

    enum Status {
        ADDED,
        REMOVED,
        MODIFIED
    };

    struct Coordinates {
        unsigned int x;
        unsigned int y;

        std::string stringify();
    };

    struct BGMEvent {
        Status status_;
        Coordinates coordinates_;

        // the following are optional
        std::string track_name_ = "";
        int volume_ = 0;
        int speed_ = 0;

        std::string stringify();
    };

    struct OpenConnection {
        Status status_;
        Coordinates coordinates_;

        std::string stringify();
    };

    struct ClosedConnection {
        Status status_;
        Coordinates coordinates_;

        std::string stringify();
    };

    struct Map {
        Status status_;
        unsigned int id_;
        std::string name_;

        std::vector<std::string> notes_;
        std::vector<BGMEvent> bgm_events_;
        std::vector<OpenConnection> open_connections_;
        std::vector<ClosedConnection> closed_connections_;

        std::string stringify();
    };

    enum ConnectionType {
        ONEWAY,
        BOTHWAY,
        UNLOCKED
    };

    struct Connection {
        Status status_;
        std::vector<std::string> notes_;

        Map from_map_;
        Coordinates from_coordinates_;

        Map to_map_;
        Coordinates to_coordinates_;

        ConnectionType type_;

        std::string stringify();
    };

    struct CommonEvent {
        Status status_;
        unsigned int id_;
        std::string name_;
        std::vector<std::string> notes_;

        std::string stringify();
    };

    struct TilesetInfo {
        Status status_;
        unsigned int id_;
        std::string name_;
        std::vector<std::string> notes_;

        std::string stringify();
    };

    struct Switch {
        Status status_;
        unsigned int id_;
        std::string name_;
        std::vector<std::string> notes_;

        std::string stringify();
    };

    struct Variable {
        Status status_;
        unsigned int id_;
        std::string name_;
        std::vector<std::string> notes_;

        std::string stringify();
    };

    struct Animation {
        Status status_;
        unsigned int id_;
        std::string name_;
        std::vector<std::string> notes_;

        std::string stringify();
    };

    enum AssetCategory {
        MENU_THEME,
        CHARSET,
        CHIPSET,
        MUSIC,
        SOUND,
        PANORAMA,
        PICTURE
    };

    struct Asset {
        Status status_;
        AssetCategory category_;
        std::string name_;
        std::string filename_;
        std::vector<std::string> notes_;

        // outside contributors only
        std::string contributors_;

        std::string stringify();
    };

    struct Changelog {
        std::string developer_;
        tm* date_;

        std::string summary_ = "";

        std::string map_policy_;
        std::string asset_policy_;

        std::vector<Map> maps_;
        std::vector<Connection> connections_;

        std::vector<CommonEvent> common_events_;

        std::vector<TilesetInfo> tilesets_;

        std::vector<Switch> switches_;

        std::vector<Variable> variables_;

        std::vector<Animation> animations_;

        std::vector<Asset> menu_themes_;

        std::vector<Asset> charsets_;

        std::vector<Asset> chipsets_;

        std::vector<Asset> musics_;

        std::vector<Asset> sounds_;

        std::vector<Asset> panoramas_;

        std::vector<Asset> pictures_;

        std::string stringify();

        Changelog() = default;
    };

}


#endif //CU_SUBMITTER_CHANGELOG_H
