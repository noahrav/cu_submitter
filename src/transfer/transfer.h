#ifndef CU_SUBMITTER_TRANSFER_H
#define CU_SUBMITTER_TRANSFER_H

#include <filesystem>

#include "../chgen/chgen.h"

namespace fs = std::filesystem;

namespace transfer {

    class DevbuildModifTransfer {
    public:
        static void transfer(const std::string& modified_path, const std::string& new_path);
    
    private:
        static std::shared_ptr<data::Changelog> changelog_;
    };

} // transfer

#endif //CU_SUBMITTER_TRANSFER_H
