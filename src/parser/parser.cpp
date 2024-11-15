module;

#include "commands.h"
#include <iostream>

export module parser;

namespace
{
parser::Command get_command(char* arg)
{
    std::cout << "arg = " << arg << "\n";
    return parser::Command::UKNOWN;
}
} // namespace

namespace parser
{
export void parse(int argc, char* argv[]) // NOLINT
{
    for (int i = 1; i < argc; i++)
    {
        get_command(argv[i]); /// NOLINT
    }
}
} // namespace parser
