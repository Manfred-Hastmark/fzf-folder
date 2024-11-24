
#include <array>
#include <csignal>
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <variant>

import parser;
import finder;

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
 * Handles unexpected program exits
 * and dismantles all resources
 */
void signal_handler(int signum)
{
    endwin();
    exit(signum);
}

/**
 * Creates a curses Window with desired options
 */
void init_curses()
{
    initscr();            // Create window
    cbreak();             // Enable continous reading
    noecho();             // Don't echo user input
    keypad(stdscr, TRUE); // Enable keypad
}
} // namespace

int main(int argc, char* argv[])
{
    auto args = parser::get_args(argc, argv);

    for (auto sig : EXIT_SIGNALS)
    {
        (void)signal(sig, signal_handler);
    }
    init_curses();

    finder::Finder finder(args.path);
    while (true)
    {
        auto input = parser::get_input();
        if (!input)
        {
            continue;
        }
        if (const auto* match = std::get_if<char>(&input.value()))
        {
            finder.update_search(*match);
        }
        else if (const auto* index = std::get_if<int>(&input.value()))
        {
            finder.update_index(*index);
        }
        else if (const auto* finish = std::get_if<bool>(&input.value()))
        {
            endwin();
            if (*finish)
            {
                std::cout << finder.get_match();
            }
            return 0;
        }
        refresh();
    }
}
