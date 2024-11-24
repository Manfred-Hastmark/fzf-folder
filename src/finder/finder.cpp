module;

#include <barrier>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <stop_token>
#include <string>
#include <thread>
#include <utility>
#include <vector>

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
    }

    /**
     * Append character to search
     * @param new_char character to append
     */
    void update_search(char new_char)
    {
        constexpr int DELETE = 127;
        if (new_char == DELETE)
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
    void find_folders(const std::stop_token& stop_token);

    tui::Tui* m_tui_p;
    fs::path m_root;
    std::string m_search;
    uint32_t m_index{0};

    std::jthread m_search_thread;
    std::barrier<> m_input_barrier;

    std::string m_match{"NOT FOUND"};
};

void Finder::find_folders(const std::stop_token& stop_token)
{
    auto folder_iter = fs::recursive_directory_iterator(m_root);
    std::set<fs::path> folders;
    std::set<std::string> matches;
    for (const auto& entry : folder_iter)
    {
        if (fs::status(entry.path()).type() == fs::file_type::directory)
        {
            folders.insert(entry.path());
            matches.insert(entry.path().string());
        }
    }
    m_tui_p->draw_matches(matches, folders.size());

    while (!stop_token.stop_requested())
    {
        m_input_barrier.arrive_and_wait();
        if (stop_token.stop_requested())
        {
            return;
        }
        {
            for (const auto& folder : folders)
            {
                if (folder.string().find(m_search) != std::string::npos)
                {
                    matches.insert(folder.string());
                }
                else
                {
                    matches.erase(folder.string());
                }
            }
            m_tui_p->draw_matches(matches, folders.size());
        }
    }
}

} // namespace finder
