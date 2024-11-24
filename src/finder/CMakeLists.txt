add_library(finder)
add_library(fzf-folder::finder ALIAS finder)

target_sources(finder
    PUBLIC
        FILE_SET CXX_MODULES FILES 
            finder.cpp
)
target_link_libraries(finder PRIVATE fzf-folder::tui)
