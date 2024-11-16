module;

#include <cstdint>
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

    void update_search(std::string search)
    {
        m_search = std::move(search);
    }

    void update_index(uint32_t index)
    {
        m_index = index;
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
