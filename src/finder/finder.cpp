module;

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

export module finder;
import tui;

namespace finder
{
/**
 * Class to handle searching
 */
export class Finder
{
  public:
    /**
     * @param tui_p terminal user interface
     * @param root path to search from
     * @param search initial search string
     */
    explicit Finder(tui::Tui* tui_p, std::filesystem::path root, std::string search = "") : m_tui_p(tui_p), m_root(std::move(root)), m_search(std::move(search))
    {
        m_tui_p->draw_input(m_search);
    }

    /**
     * Append character to search
     * @param new_char character to append
     */
    void update_search(char new_char)
    {
        m_search.push_back(new_char);
        m_tui_p->draw_input(m_search);
    }

    /**
     * Inc/Dec selected item
     * @param inc if<0 => item--, if>0 => item++
     */
    void update_index(int inc)
    {
        if (inc < 0 && m_index != 0)
        {
            m_index--;
            return;
        }
        if (inc > 0 && m_index != UINT32_MAX)
        {
            m_index++;
            return;
        }
    }

    /**
     * Retrieves the searched element
     * @return std::string selected search match
     */
    [[nodiscard]] std::string get_match() const
    {
        return m_match;
    }

  private:
    tui::Tui* m_tui_p;
    std::filesystem::path m_root;
    std::string m_search;
    uint32_t m_index{0};

    std::string m_match{"NOT FOUND"};
};
} // namespace finder
