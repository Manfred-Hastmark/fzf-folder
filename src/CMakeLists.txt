add_subdirectory(tui)
add_subdirectory(parser)
add_subdirectory(finder)

# Main target
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fzf-folder::parser)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fzf-folder::finder)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fzf-folder::tui)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -lncurses)

