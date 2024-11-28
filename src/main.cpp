
#include <array>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <filesystem>
#include <iostream>
#include <termios.h>
#include <variant>

import parser;
import finder;
import tui;

namespace
{

/**
 * Exit signals to handle and cleanup resources on
 */
constexpr std::array EXIT_SIGNALS{
    SIGTERM,
    SIGINT,
    SIGHUP,
    SIGQUIT,
    SIGABRT,
    SIGPIPE,
};

/**
 * Creates a curses Window with desired options
 */
void setup(FILE*& tty_p, termios& orig_tty)
{
    tui::Tui<>::setup(tty_p, orig_tty);
}

/**
 * Handles unexpected program exits
 * and dismantles all resources
 */
void teardown(FILE*& tty_p, termios& orig_tty)
{
    tui::Tui<>::teardown(tty_p, orig_tty);
}

} // namespace

int main(int argc, const char* argv[])
{
    static FILE* tty_p = nullptr;
    static termios orig_tty;
    for (auto sig : EXIT_SIGNALS)
    {
        (void)signal(sig, [](int signum) {
            teardown(tty_p, orig_tty);
            exit(signum);
        });
    }
    setup(tty_p, orig_tty);
    tui::Tui tui;

    try
    {
        auto args = parser::get_args(argc, argv);
        finder::Finder finder(tui, args.path);
        while (true)
        {
            auto input = parser::get_input(tui);
            if (!input)
            {
                continue;
            }
            if (const auto* match = std::get_if<char>(&input.value()))
            {
                finder.update_search(*match, tui);
            }
            else if (const auto* index = std::get_if<int>(&input.value()))
            {
                finder.update_index(*index);
            }
            else if (const auto* finish = std::get_if<bool>(&input.value()))
            {
                teardown(tty_p, orig_tty);
                if (*finish)
                {
                    std::cout << finder.get_match() << "\n";
                }
                return 0;
            }
        }
    }
    catch (parser::CmdExcept& cmd_except)
    {
        teardown(tty_p, orig_tty);
        parser::print_except(cmd_except);
    }
}
