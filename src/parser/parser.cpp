module;

#include <cassert>
#include <csignal>
#include <curses.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

export module parser;
import tui;

namespace fs = std::filesystem;

namespace parser
{
/**
 * CLI args
 */
export enum class Command : uint8_t
{
    UKNOWN,
    ICASE, // Case insensitive (-i)
    PATH,  // Arg is a path
    HELP,  // Help (-h)
};
} // namespace parser

namespace
{
/**
 * Parses cli parameter as flag
 * @param arg string with flag
 */
[[nodiscard]] parser::Command get_command(const char* arg)
{
    if (fs::exists(arg))
    {
        return parser::Command::PATH;
    }
    if (std::string("-i") == arg)
    {
        return parser::Command::ICASE;
    }
    if (std::string("-h") == arg)
    {
        return parser::Command::HELP;
    }
    return parser::Command::UKNOWN;
}

/**
 * Print help list
 */
void print_help()
{
    std::cout << "Usage:\n"
                 " - fzf-folder          -Runs tool with current directory as root\n"
                 " - fzf-folder <path>   -Runs tool with <path> as root-directory\n"
                 " - fzf-folder -i       -Case insensitive search\n"
                 " - fzf-folder -f       -Printout full path and not relative\n"
                 " - fzf-folder -h       -Print this help page\n";
}

/**
 * Print unkown arg
 */
void print_unknown(const char* arg)
{
    std::cout << "Unrecognized arg: " << arg
              << "\n"
                 " - Run fzf-folder -h for a full list of commands\n";
}

/**
 * Represents program argument
 */
struct Args
{
    fs::path path;
    std::vector<parser::Command> commands;
};
} // namespace

namespace parser
{

/**
 * Represents exception thrown when parsing commands
 */
export class CmdExcept : public std::exception
{
  public:
    explicit CmdExcept(Command type, const char* arg = "") : m_type(type), m_arg(arg)
    {
    }

    [[nodiscard]] Command type() const
    {
        return m_type;
    }

    [[nodiscard]] const char* arg() const
    {
        return m_arg;
    }

  private:
    Command m_type;
    const char* m_arg;
};

/**
 * Prints help text for commands
 * @param except exceptions raised
 */
export void print_except(const CmdExcept& except) /// NOLINT
{
    switch (except.type())
    {
    case Command::HELP:
        print_help();
        break;
    default:
        print_unknown(except.arg());
        break;
    }
}

/**
 * Parses command line arguments to Args
 * @param argc number of arguments
 * @param argv list of arguments
 * @see Args()
 */
export [[nodiscard]] Args get_args(int argc, const char* argv[]) /// NOLINT
{
    Args args{
        .path = fs::current_path(),
        .commands = {},
    };
    for (int i = 1; i < argc; i++)
    {
        auto command = get_command(argv[i]); /// NOLINT
        switch (command)
        {
        case Command::PATH:
            args.path = fs::path(argv[i]); /// NOLINT
            break;
        case Command::HELP:
            throw CmdExcept(command);
            break;
        case Command::UKNOWN:
            throw CmdExcept(command, argv[i]); /// NOLINT
            break;
        default:
            args.commands.push_back(command);
            break;
        }
    }
    assert(fs::exists(args.path));
    return args;
}

/**
 * Retrieves user input
 * @param tui terminal ui to get user input
 * @return char if the user gave a match, 0 if backspace
 *         bool if the user escaped or entered,
 *         int if the user navigated up or down {-1, 1}
 */
export [[nodiscard]] std::optional<std::variant<char, bool, int>> get_input(const auto& tui) /// NOLINT
{
    auto input = tui.get_input();

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

    constexpr int DELETE = 263;
    if (input == DELETE)
    {
        return static_cast<char>(0);
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
