
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <optional>
#include <variant>

import stubTui;
import parser;
import tui;

struct InputIO
{
    int input{};
    std::optional<std::variant<char, bool, int>> output;
};

class TestParser : public testing::TestWithParam<InputIO>
{
  private:
    void SetUp() override
    {
        stubTui::MockImpl::create_mock();
    };

    void TearDown() override
    {
        stubTui::MockImpl::delete_mock();
    }
};

TEST_P(TestParser, testGetInput)
{
    auto [input, output] = GetParam();

    EXPECT_CALL(*stubTui::MockImpl::get_mock(), get_input()).WillOnce(testing::Return(input));

    tui::Tui<stubTui::MockImpl> tui;

    EXPECT_EQ(parser::get_input(tui), output);
}

INSTANTIATE_TEST_SUITE_P(SweepGetInput,
                         TestParser,
                         testing::Values(
                             InputIO{
                                 .input = 0, // Unvalid
                                 .output = std::nullopt,
                             },
                             InputIO{
                                 .input = 10, // Enter
                                 .output = true,
                             },
                             InputIO{
                                 .input = 27, // Escape
                                 .output = false,
                             },
                             InputIO{
                                 .input = 259, // Arrow Up
                                 .output = 1,
                             },
                             InputIO{
                                 .input = 9, // Tab
                                 .output = 1,
                             },
                             InputIO{
                                 .input = 258, // Arrow Down
                                 .output = -1,
                             },
                             InputIO{
                                 .input = 353, // Shift+Tab
                                 .output = -1,
                             },
                             InputIO{
                                 .input = 263, // Backspace
                                 .output = '\0',
                             },
                             InputIO{
                                 .input = 'h', // Some Letter
                                 .output = 'h',
                             }));
