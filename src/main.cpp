
#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

import parser;
import finder;

int main(int argc, char* argv[])
{
    auto args = parser::get_args(argc, argv);
    finder::Finder finder(args.path);
    while (true)
    {
        auto input = parser::get_input();
        if (const auto* match = std::get_if<std::string>(&input))
        {
            finder.update_search(*match);
        }
        else if (const auto* index = std::get_if<uint32_t>(&input))
        {
            finder.update_index(*index);
        }
        else if (const auto* finish = std::get_if<bool>(&input))
        {
            if (*finish)
            {
                std::cout << finder.get_match();
            }
            return 0;
        }
    }
}
