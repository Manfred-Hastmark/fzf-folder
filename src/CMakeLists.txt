cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
project(fzf-folder LANGUAGES CXX)

add_executable(fzf-folder main.cpp)


add_subdirectory(parser)
target_link_libraries(fzf-folder PRIVATE fzf-folder::parser)
