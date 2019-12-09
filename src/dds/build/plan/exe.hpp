#pragma once

#include <dds/build/plan/compile_file.hpp>
#include <dds/util/fs.hpp>

#include <string>
#include <vector>

namespace dds {

class library_plan;

/**
 * Represents information about a test failure.
 */
struct test_failure {
    fs::path    executable_path;
    std::string output;
    int         retc;
};

/**
 * Stores information about an executable that should be linked. An executable in DDS consists of a
 * single source file defines the entry point and some set of linker inputs.
 */
class link_executable_plan {
    /// The linker inputs that should be linked into the executable
    std::vector<fs::path> _input_libs;
    /// The compilation plan for the entry-point source file
    compile_file_plan _main_compile;
    /// The subdirectory in which the executable should be generated
    fs::path _out_subdir;
    /// The name of the executable
    std::string _name;

public:
    /**
     * Create a new instance
     * @param in_libs Linker inputs for the executable
     * @param cfp The file compilation that defines the entrypoint of the application
     * @param out_subdir The subdirectory of the build root in which the executable should be placed
     * @param name_ The name of the executable
     */
    link_executable_plan(std::vector<fs::path> in_libs,
                         compile_file_plan     cfp,
                         path_ref              out_subdir,
                         std::string           name_)
        : _input_libs(std::move(in_libs))
        , _main_compile(std::move(cfp))
        , _out_subdir(out_subdir)
        , _name(std::move(name_)) {}

    /**
     * Get the compilation of the main source file
     */
    auto& main_compile_file() const noexcept { return _main_compile; }

    /**
     * Calculate the output path of the executable for the given build environment
     */
    fs::path calc_executable_path(const build_env& env) const noexcept;

    /**
     * Perform the link of the executable
     * @param env The build environment to use.
     * @param lib The library that owns this executable. If it defines an archive library, it will
     * be added as a linker input.
     */
    void link(const build_env& env, const library_plan& lib) const;
    /**
     * Run the executable as a test. If the test fails, then that failure information will be
     * returned.
     */
    std::optional<test_failure> run_test(build_env_ref) const;

    bool is_test() const noexcept;
    bool is_app() const noexcept;
};

}  // namespace dds