#pragma once

#include <dds/util/fs.hpp>

#include <vector>

namespace dds {

enum class deps_mode {
    none,
    msvc,
    gnu,
};

struct deps_info {
    fs::path              output;
    std::vector<fs::path> inputs;
    std::string           command;
    std::string           command_output;
};

}  // namespace dds