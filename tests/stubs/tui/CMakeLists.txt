add_library(stub_tui)
add_library(fzf-folder::stubs::tui ALIAS stub_tui)

target_sources(stub_tui
    PUBLIC
        FILE_SET CXX_MODULES FILES 
            stub_tui.cpp
)

find_package(GTest CONFIG REQUIRED COMPONENTS GMock)
target_link_libraries(stub_tui PRIVATE GTest::gmock)
