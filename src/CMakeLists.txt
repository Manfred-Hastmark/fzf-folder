cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
project(fzf-folder LANGUAGES CXX)

add_executable(fzf-folder main.cpp)

add_subdirectory(parser)
target_link_libraries(fzf-folder PRIVATE fzf-folder::parser)

add_subdirectory(finder)
target_link_libraries(fzf-folder PRIVATE fzf-folder::finder)

add_subdirectory(tui)
target_link_libraries(fzf-folder PRIVATE fzf-folder::tui)

target_link_options(fzf-folder PRIVATE -lncurses)
