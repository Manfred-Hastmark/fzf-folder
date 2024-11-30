
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

import stubTui;
import parser;
import tui;

/**
 * Struct representing IO for get_input
 */
struct InputIO
{
    int input{};
    std::optional<std::variant<char, bool, int>> output;
};

/**
 * Testclass for testing get_input
 */
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

/**
 * Parameterized test checking IO of get_input for different keys
 */
TEST_P(TestParser, testGetInput)
{
    auto [input, output] = GetParam();

    EXPECT_CALL(*stubTui::MockImpl::get_mock(), get_input()).WillOnce(testing::Return(input));

    tui::Tui<stubTui::MockImpl> tui;

    EXPECT_EQ(parser::get_input(tui), output);
}

/**
 * Inputed key and expected output value for get_input
 */
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

/**
 * Struct representing IO for get_args
 */
struct ArgsIO
{
    std::vector<const char*> args;
    std::filesystem::path path;
    std::vector<parser::Command> commands;
};

/**
 * Test structure for get_args
 */
class TestGetArgs : public testing::TestWithParam<ArgsIO>
{
  public:
    static std::string argsToString(const std::vector<const char*>& args)
    {
        std::string args_string("[");
        if (!args.empty())
        {
            for (auto iter = args.begin(); iter != args.end() - 1; iter++)
            {
                args_string.append(*iter);
                args_string.append(", ");
            }
            args_string.append(args.back());
        }
        args_string.append("]");
        return args_string;
    }

    static std::string commandsToString(const std::vector<parser::Command>& cmds)
    {
        static std::map<parser::Command, std::string> cmd_to_string{
            {parser::Command::UKNOWN, "Command::UNKOWN"},
            {parser::Command::HELP, "Command::HELP"},
            {parser::Command::PATH, "Command::PATH"},
            {parser::Command::ICASE, "Command::ICASE"},
            {parser::Command::FPATH, "Command::FPATH"},
        };

        std::string cmds_string("[");
        if (!cmds.empty())
        {
            for (auto iter = cmds.begin(); iter != cmds.end() - 1; iter++)
            {
                cmds_string.append(cmd_to_string.at(*iter));
                cmds_string.append(", ");
            }
            cmds_string.append(cmd_to_string.at(cmds.back()));
        }
        cmds_string.append("]");
        return cmds_string;
    }
};

/**
 * Parameterized test for get_args
 */
TEST_P(TestGetArgs, testGetArgs)
{
    auto [args, path, commands] = GetParam();

    try
    {
        auto out = parser::get_args(static_cast<int>(args.size()), args.data());
        EXPECT_EQ(out.path, path) << "Expected path does not math parsed path\n"
                                     "Expected path: "
                                  << path
                                  << "\n"
                                     "Parsed path: "
                                  << out.path;
        EXPECT_EQ(out.commands, commands) << "Expected commands don't match parsed commands\n"
                                             "Expected commands: "
                                          << commandsToString(commands)
                                          << "\n"
                                             "Parsed commands: "
                                          << commandsToString(out.commands)
                                          << "\n"
                                             "Provided args: "
                                          << argsToString(args);
    }
    catch (parser::CmdExcept& except)
    {
        bool help{false};
        for (const auto* arg : args)
        {
            help |= std::string(arg).find("-h") != std::string::npos;
        }
        if (help)
        {
            EXPECT_EQ(except.type(), parser::Command::HELP) << "Flag -h was provided\n"
                                                               "Expected exception type: Command::HELP";
        }
        else
        {
            EXPECT_EQ(except.type(), parser::Command::UKNOWN) << "Got exception and -h flag was not present\n"
                                                                 "Expected exception type: Command::UNKOWN";
        }
        EXPECT_EQ(path, std::filesystem::path("")) << "Received command except, no path parsed\n"
                                                      "But expected: "
                                                   << path;
        EXPECT_EQ(commands.empty(), true) << "Received command except, no commands parsed\n"
                                             "But expected: "
                                          << commandsToString(commands);
    }
}

/**
 * Test cases for get_args, with given input and expected output
 */
INSTANTIATE_TEST_SUITE_P(SweetGetArgs,
                         TestGetArgs,
                         testing::Values(
                             ArgsIO{
                                 .args{"fzf-folder", "."},
                                 .path{std::filesystem::path(".")},
                                 .commands{},
                             },
                             ArgsIO{
                                 .args = {"fzf-folder", "-i", "-f"},
                                 .path{std::filesystem::current_path()},
                                 .commands{parser::Command::ICASE, parser::Command::FPATH},
                             },
                             ArgsIO{
                                 .args = {"fzf-folder", "-h"},
                                 .path{},
                                 .commands{},
                             },
                             ArgsIO{
                                 .args = {"fzf-folder", "random-unkown-command"},
                                 .path{},
                                 .commands{},
                             }));
