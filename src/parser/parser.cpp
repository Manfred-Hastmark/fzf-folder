module;

#include "commands.h"
#include <cassert>
#include <curses.h>
#include <filesystem>
#include <optional>
#include <variant>
#include <vector>

export module parser;

namespace fs = std::filesystem;

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
    fs::path path;
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
        .path = fs::path(fs::current_path()),
        .commands = {},
    };
    if (argc > 1)
    {
        args.path = fs::path(argv[1]); /// NOLINT
    }
    assert(fs::exists(args.path));
    for (int i = 2; i < argc; i++)
    {
        args.commands.push_back(get_command(argv[i])); /// NOLINT
    }
    return args;
}

/**
 * Retrieves user input
 * @return char if the user gave a match,
 *         bool if the user escaped or entered,
 *         int if the user navigated up or down {-1, 1}
 */
export [[nodiscard]] std::optional<std::variant<char, bool, int>> get_input() /// NOLINT
{
    auto input = getch();

    constexpr int ENTER = 10;
    if (input == ENTER)
    {
        return true;
    }

    constexpr int ESCAPE = 27;
    if (input == ESCAPE)
    {
        return false;
    }

    constexpr int ARROW_UP = 259;
    constexpr int TAB = 9;
    if (input == ARROW_UP || input == TAB)
    {
        return 1;
    }

    constexpr int ARROW_DOWN = 258;
    constexpr int SHIFT_TAB = 353;
    if (input == ARROW_DOWN || input == SHIFT_TAB)
    {
        return -1;
    }

    constexpr int MIN_PRINTABLE_ASCII = 32;
    constexpr int MAX_PRINTABLE_ASCII = 126;
    if (MIN_PRINTABLE_ASCII <= input && input <= MAX_PRINTABLE_ASCII)
    {
        return static_cast<char>(input);
    }
    return std::nullopt;
}
} // namespace parser
