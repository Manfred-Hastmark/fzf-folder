module;

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

export module finder;

namespace finder
{
export class Finder
{
  public:
    explicit Finder(std::filesystem::path root, std::string search = "") : m_root(std::move(root)), m_search(std::move(search))
    {
    }

    void update_search(char new_char)
    {
        m_search.push_back(new_char);
    }

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

    [[nodiscard]] std::string get_match() const
    {
        return m_match;
    }

  private:
    std::filesystem::path m_root;
    std::string m_search;
    uint32_t m_index{0};

    std::string m_match{"NOT FOUND"};
};
} // namespace finder
