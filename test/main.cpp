
#include "termios.h"
#include <cstdio>
#include <cstring>
#include <curses.h>
#include <iostream>
#include <ostream>
#include <unistd.h>

int main()
{
    struct termios orig_tty{};
    FILE* tty = fopen("/dev/tty", "r+");
    tcgetattr(fileno(tty), &orig_tty);

    initscr();
    newterm(nullptr, tty, tty);

    printw("Give me something:\n");
    refresh();
    char input[100];
    getstr(input);
    endwin();

    tcsetattr(fileno(tty), 0, &orig_tty);
    fclose(tty);
    std::cout << "Hello\n";
}
