module;

#include <barrier>
#include <cstddef>
#include <cstdlib>
#include <curses.h>
#include <filesystem>
#include <set>
#include <stop_token>
#include <string>
#include <thread>
#include <utility>

export module finder;
import tui;

namespace fs = std::filesystem;

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
    explicit Finder(tui::Tui* tui_p, fs::path root, std::string search = "")
        : m_tui_p(tui_p), m_root(std::move(root)), m_search(std::move(search)), m_search_thread([this](const std::stop_token& stop_token) { find_folders(stop_token); }), m_input_barrier(2)
    {
        m_tui_p->draw_input(m_search);
    }

    ~Finder()
    {
        m_search_thread.request_stop();
        m_input_barrier.arrive_and_drop();
        m_search_thread.join();
    }

    /**
     * Append character to search
     * @param new_char character to append
     */
    void update_search(char new_char)
    {
        if (new_char == 0 && !m_search.empty())
        {
            m_search.pop_back();
        }
        else
        {
            m_search.push_back(new_char);
        }
        m_input_barrier.arrive_and_wait();
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
        }
        else if (inc > 0 && m_index != m_matches.size())
        {
            m_index++;
        }
        m_input_barrier.arrive_and_wait();
    }

    /**
     * Retrieves the searched element
     * @return std::string selected search match
     */
    [[nodiscard]] const std::string& get_match() const
    {
        return m_match;
    }

  private:
    void find_folders(const std::stop_token& stop_token);

    tui::Tui* m_tui_p;
    fs::path m_root;
    std::string m_search;
    size_t m_index{0};
    std::string m_match;
    std::set<std::string> m_matches;

    std::jthread m_search_thread;
    std::barrier<> m_input_barrier;
};

void Finder::find_folders(const std::stop_token& stop_token)
{
    auto folder_iter = fs::recursive_directory_iterator(m_root);
    std::set<std::string> non_matches;
    for (const auto& entry : folder_iter)
    {
        if (fs::status(entry.path()).type() == fs::file_type::directory)
        {
            m_matches.insert(fs::relative(entry.path(), m_root));
        }
    }
    m_match = *m_matches.begin();
    m_tui_p->draw_matches(m_index, m_matches, m_matches.size());

    while (!stop_token.stop_requested())
    {
        m_input_barrier.arrive_and_wait();
        if (stop_token.stop_requested())
        {
            break;
        }
        {
            for (auto iter = non_matches.begin(); iter != non_matches.end();)
            {
                if (iter->find(m_search) != std::string::npos)
                {
                    m_matches.insert(*iter);
                    iter = non_matches.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            size_t index{0};
            for (auto iter = m_matches.begin(); iter != m_matches.end();)
            {
                if (iter->find(m_search) == std::string::npos)
                {
                    non_matches.insert(*iter);
                    iter = m_matches.erase(iter);
                }
                else
                {
                    if (index == m_index)
                    {
                        m_match = *iter;
                    }
                    index++;
                    iter++;
                }
            }
            if (m_matches.size() <= m_index)
            {
                m_index = m_matches.size() - 1;
            }
            m_tui_p->draw_matches(m_index, m_matches, m_matches.size() + non_matches.size());
        }
    }
}
} // namespace finder
