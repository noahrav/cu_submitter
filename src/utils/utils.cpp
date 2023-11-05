#include "utils.h"


namespace utils {

    bool compareFiles(const std::string& path1, const std::string& path2) {
        std::ifstream img1(path1, std::ios::binary);
        std::ifstream img2(path2, std::ios::binary);

        if (!img1.is_open() || !img2.is_open()) {
            error("Could not open image file " + (img1.is_open() ? path1 : path2) + " for comparison");
            return false;
        }

        img1.seekg(0, std::ios::end);
        img2.seekg(0, std::ios::end);

        const auto img1_size = img1.tellg();
        const auto img2_size = img2.tellg();

        if (img1_size != img2_size) {
            return false;
        }

        img1.seekg(0, std::ios::beg);
        img2.seekg(0, std::ios::beg);

        std::vector<char> img1_data(img1_size);
        std::vector<char> img2_data(img2_size);

        img1.read(img1_data.data(), img1_size);
        img2.read(img2_data.data(), img2_size);

        return img1_data == img2_data;
    }

}