module;

#include <cstddef>
#include <curses.h>
#include <mutex>
#include <set>
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
        keypad(m_winput_p, TRUE);
    };

    /**
     * Draws the user input
     * @param path directory to draw in front off input
     * @param input search string
     */
    void draw_input(const std::string& input)
    {
        m_term_mutex.lock();
        wmove(m_winput_p, 0, 0);
        wclrtoeol(m_winput_p);
        wprintw(m_winput_p, "> ");
        wprintw(m_winput_p, "%s", input.c_str());
        getyx(m_winput_p, m_winput_pos.y, m_winput_pos.x);
        wrefresh(m_winput_p);
        m_term_mutex.unlock();
    }

    /**
     * Draws the matching folders
     * @param matches vector with all folder names
     * @param total_folders total amount of folders in search dir
     */
    void draw_matches(size_t index, const std::set<std::string>& matches, size_t total_folders)
    {
        m_term_mutex.lock();
        int height = getmaxy(m_wresults_p);
        wmove(m_wresults_p, 0, 0);
        werase(m_wresults_p);
        wmove(m_wresults_p, height - 1, 0);
        wprintw(m_wresults_p, " %zu/%zu", matches.size(), total_folders);
        height--;
        wmove(m_wresults_p, height - 1, 0);
        size_t iter_index{0};
        for (const auto& match : matches)
        {
            if (iter_index == index)
            {
                wattron(m_wresults_p, A_STANDOUT);
                wprintw(m_wresults_p, "  %s", match.c_str());
                wattroff(m_wresults_p, A_STANDOUT);
            }
            else
            {
                wprintw(m_wresults_p, " %s", match.c_str());
            }
            height--;
            if (height == 0)
            {
                break;
            }
            wmove(m_wresults_p, height - 1, 0);
            iter_index++;
        }
        wrefresh(m_wresults_p);
        wrefresh(m_winput_p);
        m_term_mutex.unlock();
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
    std::mutex m_term_mutex;
};
} // namespace tui
