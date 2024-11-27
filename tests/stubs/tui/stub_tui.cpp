module;

#include "gmock/gmock.h"
#include <cstddef>
#include <curses.h>
#include <memory>
#include <set>
#include <string>

export module stubTui;

namespace stubTui
{
class MockTui
{
  public:
    MOCK_METHOD(void, draw_input, (const std::string& input), ());
    MOCK_METHOD(void, draw_matches, (size_t, const std::set<std::string>&, size_t), ());
    MOCK_METHOD(int, get_input, (), (const));
};
static std::unique_ptr<MockTui> mock_up; /// NOLINT

export class MockImpl
{
  public:
    MockImpl() = default;

    void static draw_input(const std::string& input)
    {
        mock_up->draw_input(input);
    }

    void static draw_matches(size_t index, const std::set<std::string>& matches, size_t total_folders)
    {
        mock_up->draw_matches(index, matches, total_folders);
    }

    [[nodiscard]] int static get_input()
    {
        return mock_up->get_input();
    }

    static void create_mock()
    {
        if (mock_up == nullptr)
        {
            mock_up = std::make_unique<MockTui>();
        }
    }

    static void delete_mock()
    {
        if (mock_up != nullptr)
        {
            mock_up.reset();
        }
    }

    static MockTui* get_mock()
    {
        return mock_up.get();
    }
};
} // namespace stubTui
