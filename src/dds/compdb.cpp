#include "./compdb.hpp"

#include <dds/build/iter_compilations.hpp>
#include <dds/proc.hpp>
#include <dds/util/fs.hpp>

#include <nlohmann/json.hpp>

using namespace dds;

void dds::generate_compdb(const build_plan& plan, build_env_ref env) {
    auto compdb = nlohmann::json::array();

    for (const compile_file_plan& cf : iter_compilations(plan)) {
        auto cmd_info = cf.generate_compile_command(env);
        auto entry    = nlohmann::json::object({
            {"directory", env.output_root.string()},
            {"arguments", cmd_info.command},
            {"file", cf.source_path().string()},
        });
        compdb.push_back(std::move(entry));
    }

    fs::create_directories(env.output_root);
    auto compdb_file = env.output_root / "compile_commands.json";
    auto ostream     = open(compdb_file, std::ios::binary | std::ios::out);
    ostream << compdb.dump(2);
}