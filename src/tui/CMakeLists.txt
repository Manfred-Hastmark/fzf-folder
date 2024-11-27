add_library(tui)
add_library(fzf-folder::tui ALIAS tui)

target_sources(tui
    PUBLIC
        FILE_SET CXX_MODULES FILES 
            tui.cpp
)
