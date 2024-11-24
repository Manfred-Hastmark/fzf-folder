add_library(parser)
add_library(fzf-folder::parser ALIAS parser)

target_sources(parser
    PUBLIC
        FILE_SET CXX_MODULES FILES 
            parser.cpp
)

target_link_libraries(parser PRIVATE fzf-folder::tui)
