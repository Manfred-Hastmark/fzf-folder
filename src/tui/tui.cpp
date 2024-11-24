module;

#include <curses.h>
#include <string>

export module tui;

namespace tui
{
constexpr int WINPUT_HEIGHT{1};

/**
 * class to represent terminal user interface
 */
export class Tui
{
  public:
    Tui() : m_winput_p(newwin(WINPUT_HEIGHT, COLS, LINES - WINPUT_HEIGHT, 0)), m_wresults_p(newwin(LINES - WINPUT_HEIGHT, COLS, 0, 0))
    {
        wrefresh(m_winput_p);
        wrefresh(m_wresults_p);
    };

    /**
     * Draws the user input
     * @param path directory to draw in front off input
     * @param input search string
     */
    void draw_input(const std::string& input)
    {
        wmove(m_winput_p, 0, 0);
        wprintw(m_winput_p, "> ");
        wprintw(m_winput_p, "%s", input.c_str());
        getyx(m_winput_p, m_winput_pos.y, m_winput_pos.x);
        wrefresh(m_winput_p);
    }

    /**
     * Uses getch to retrive user input
     * @return input character
     */
    [[nodiscard]] int get_input() const
    {
        wmove(m_winput_p, m_winput_pos.y, m_winput_pos.x);
        wrefresh(m_winput_p);
        return wgetch(m_winput_p);
    }

  private:
    struct Pos
    {
        int y{0};
        int x{0};
    };

    Pos m_winput_pos;
    WINDOW* m_winput_p;
    WINDOW* m_wresults_p;
};
} // namespace tui
