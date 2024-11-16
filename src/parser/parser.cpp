module;

#include "commands.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

export module parser;

namespace
{
[[nodiscard]] parser::Command get_command(char* /* arg */)
{
    return parser::Command::UKNOWN;
}
} // namespace

namespace parser
{
/**
 * Represents program argument
 */
struct Args
{
    std::filesystem::path path;
    std::vector<Command> commands;
};

/**
 * Parses command line arguments to Args
 * @param argc number of arguments
 * @param argv list of arguments
 * @see Args()
 */
export [[nodiscard]] Args get_args(int argc, char* argv[]) /// NOLINT
{
    Args args{
        .path = std::filesystem::path(argv[1]), /// NOLINT
        .commands = {},
    };
    for (int i = 2; i < argc; i++)
    {
        args.commands.push_back(get_command(argv[i])); /// NOLINT
    }
    return args;
}

/**
 * Retrieves user input
 * @return std::string if the user gave a match,
 *         bool if the user escaped or entered,
 *         uint32_t if the user navigated up or down
 */
export [[nodiscard]] std::variant<std::string, bool, uint32_t> get_input() /// NOLINT
{
    return "";
}
} // namespace parser
