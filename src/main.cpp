
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

FILE* tty = nullptr;
termios orig_tty;

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
    tcsetattr(fileno(tty), 0, &orig_tty);
    fclose(tty);
    exit(signum);
}

/**
 * Creates a curses Window with desired options
 */
void init_curses()
{
    tty = fopen("/dev/tty", "r+");
    tcgetattr(fileno(tty), &orig_tty);

    initscr(); // Create window
    cbreak();  // Enable continous reading
    noecho();  // Don't echo user input
    auto* screen = newterm(nullptr, tty, tty);
    set_term(screen);
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
    tui::Tui tui;
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
            endwin();
            tcsetattr(fileno(tty), 0, &orig_tty);
            fclose(tty);
            if (*finish)
            {
                std::cout << finder.get_match() << "\n";
            }
            return 0;
        }
    }
}
