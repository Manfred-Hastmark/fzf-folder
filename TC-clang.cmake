set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-std=c++20 -Wpedantic -Wall -Wextra -Werror -Wshadow -Wdeprecated -Wconversion")
