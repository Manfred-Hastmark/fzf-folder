add_executable(test-parser test_parser.cpp)
add_test(NAME TestParser COMMAND test-parser)

target_link_libraries(test-parser PRIVATE fzf-folder::parser)
target_link_libraries(test-parser PRIVATE fzf-folder::stubs::tui)

find_package(GTest)
target_link_libraries(test-parser PRIVATE GTest::GTest GTest::Main)
target_link_options(test-parser PRIVATE -lncurses)
